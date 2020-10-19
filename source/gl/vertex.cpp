#include "gl/vertex.hpp"

#include "glad/glad.h"

#include "gl/detail/state.hpp"
#include "gl/shader.hpp"
#include "essential/log.hpp"

clap::gl::buffer::multiple::multiple(size_t count)
	: count(count), currently_mapped_id(size_t(-1)), currently_mapped_pointer(nullptr) {
	gl::detail::state::verify_loaded();

	ids = new uint32_t[count];
	glGenBuffers(GLsizei(count), ids);
	log::message::minor << "Buffers (" << count << ") were created.";
}
clap::gl::buffer::multiple::~multiple() {
	if (currently_mapped_pointer)
		id(currently_mapped_id).unmap();

	for (size_t i = 0; i < count; i++)
		while (auto target = gl::detail::state::is_bound(id(i)))
			gl::detail::state::unbind(*target);

	auto temp_vao = gl::detail::state::unbind();

	glDeleteBuffers(GLsizei(count), ids);
	if (count) delete[] ids;
	log::message::minor << "Buffers (" << count << ") were destroyed.";

	if (temp_vao)
		gl::detail::state::bind(std::move(*temp_vao));
}

clap::gl::buffer::detail::indexed clap::gl::buffer::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

clap::gl::buffer::multiple::multiple(size_t count, uint32_t *ids, size_t currently_mapped_id,
									 void *currently_mapped_pointer) : count(count), ids(ids),
	currently_mapped_id(currently_mapped_id),
	currently_mapped_pointer(currently_mapped_pointer) {
	gl::detail::state::verify_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsBuffer(ids[i])) {
			log::warning::critical << "Unable to perform a buffer move operation. "
				"Passed buffer seems to be corrupted.";
			return;
		}

	log::message::negligible << "Buffers (" << count << ") were moved.";
}

void clap::gl::buffer::detail::indexed::bind(target target) {
	gl::detail::state::bind(target, std::move(*this));
}

void clap::gl::buffer::detail::indexed::data(size_t size, usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, nullptr, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of space were allocated for a buffer bound to '" << target << "'.";
}
void clap::gl::buffer::detail::indexed::data(void *data, size_t size,
											 usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, data, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of data were put into a buffer bound to '" << target << "'.";
}
void clap::gl::buffer::detail::indexed::subdata(void *data, size_t size,
												size_t offset, target target) {
	bind(target);
	glBufferSubData(gl::detail::convert::to_gl(target), offset, size, data);

	log::message::minor << size << " bytes of data were put into a buffer bound to '"
		<< target << "' with offset: " << offset << ".";
}

void *clap::gl::buffer::detail::indexed::map(access access, target target) {
	if (pointer->currently_mapped_pointer)
		if (index == pointer->currently_mapped_id)
			return pointer->currently_mapped_pointer;
		else
			unmap();

	bind(target);
	pointer->currently_mapped_id = index;
	pointer->currently_mapped_pointer = glMapBuffer(gl::detail::convert::to_gl(target),
													gl::detail::convert::to_gl(access));

	log::message::minor << "A buffer bound to '" << target << "' was mapped.";
	log::info::major << "The address is '" << pointer->currently_mapped_pointer << "'.";

	return pointer->currently_mapped_pointer;
}
void clap::gl::buffer::detail::indexed::unmap(target target) {
	if (pointer->currently_mapped_pointer) {
		bind(target);
		glUnmapBuffer(gl::detail::convert::to_gl(target));
		pointer->currently_mapped_pointer = nullptr;
		log::message::minor << "A buffer bound to '" << target << "' was unmapped.";
	} else
		log::warning::minor << "Attempting to unmap a pointer when none is mapped.";
}

void clap::gl::buffer::detail::indexed::copy(indexed &source, size_t size,
											 size_t read_offset, size_t write_offset) {
	bind(target::copy_write);
	source.bind(target::copy_read);
	glCopyBufferSubData(gl::detail::convert::to_gl(target::copy_read),
						gl::detail::convert::to_gl(target::copy_write),
						read_offset, write_offset, size);

	log::message::minor << "Some data was copied between two buffers.";
}
void clap::gl::buffer::detail::indexed::invalidate(size_t size, size_t offset) {
	glInvalidateBufferSubData(pointer->ids[index], offset, size);
	log::message::minor << size << " bytes of data were invalidated in a buffer "
		"with offset: " << offset << ".";
}
void clap::gl::buffer::detail::indexed::invalidate() {
	glInvalidateBufferData(pointer->ids[index]);
	log::message::minor << "A buffer was invalidated.";
}

clap::gl::buffer::detail::indexed::operator bool() const { return pointer && index < pointer->count; }
uint32_t clap::gl::buffer::detail::indexed::operator*() const { return pointer->ids[index]; }

clap::gl::buffer::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds buffer.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
}

clap::gl::vertex_array::multiple::multiple(size_t count)
	: count(count) {
	gl::detail::state::verify_loaded();

	ids = new uint32_t[count];
	glGenVertexArrays(GLsizei(count), ids);
	log::message::minor << "Vertex Arrays (" << count << ") were created.";
}
clap::gl::vertex_array::multiple::~multiple() {
	for (size_t i = 0; i < count; i++)
		if (gl::detail::state::is_bound(id(i)))
			gl::detail::state::unbind();

	glDeleteVertexArrays(GLsizei(count), ids);
	if (count) delete[] ids;
	log::message::minor << "Vertex Arrays (" << count << ") were destroyed.";
}

clap::gl::vertex_array::detail::indexed clap::gl::vertex_array::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

clap::gl::vertex_array::multiple::multiple(size_t count, uint32_t *ids) : count(count), ids(ids) {
	gl::detail::state::verify_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsVertexArray(ids[i])) {
			log::warning::critical << "Unable to perform a vertex array move operation. "
				"Passed vertex array seems to be corrupted.";
			return;
		}

	log::message::negligible << "Vertex Arrays (" << count << ") were moved.";
}

void clap::gl::vertex_array::detail::indexed::bind() {
	gl::detail::state::bind(std::move(*this));
}

template <typename lambda_t>
struct vertex_array_variable_visitor {
	lambda_t lambda;

	vertex_array_variable_visitor(lambda_t lambda) : lambda(lambda) {}

	template <typename T>
	inline void operator()(T v) {
		lambda(1, 1, sizeof(T));
	}
	template<typename T, size_t N>
	inline void operator()(clap::gl::shader::variable::type::detail::vec<T, N> v) {
		lambda(N, 1, sizeof(T));
	}
	template<typename T, size_t W, size_t H>
	inline void operator()(clap::gl::shader::variable::type::detail::mat<T, W, H> v) {
		lambda(W, H, sizeof(T));
	}
};

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																shader::variable::attribute const &variable,
																size_t stride, size_t shift) {
	this->bind();
	buffer.bind();
	std::visit(
		vertex_array_variable_visitor{
			[&](size_t width, size_t height, size_t size) {
				for (size_t i = 0; i < height; i++) {
					glVertexAttribPointer(GLuint(variable.location + i), 
										  GLint(width * height),
										  gl::detail::convert::to_gl_type(variable),
										  GL_FALSE, GLsizei(stride * size),
										  (const void *) (shift * size));
					glEnableVertexAttribArray(GLuint(variable.location + i));
				}
			}
		}, variable
	);
	log::message::minor << "A " << variable << " was bound to a "
		<< "(to replace with operator<<(ostream, buffer)!) buffer" << ".";
	log::info::minor << "Stride: " << stride << ".";
	log::info::minor << "Shift: " << shift << ".";
}

void clap::gl::vertex_array::detail::indexed::attribute_divisor(shader::variable::attribute const &variable,
																size_t divisor) {
	this->bind();
	std::visit(
		vertex_array_variable_visitor{
			[&](size_t width, size_t height, size_t size) {
				for (size_t i = 0; i < height; i++)
					glVertexAttribDivisor(GLuint(variable.location + i), GLuint(divisor));
			}
		}, variable
	);
	log::message::minor << "A " << variable << " uses a divisor: " << divisor << ".";
}

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																shader::variable::attribute const &variable,
																size_t stride, size_t shift,
																size_t divisor) {
	attribute_pointer(std::move(buffer), variable, stride, shift);
	attribute_divisor(variable, divisor);
}

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																std::initializer_list<shader::variable::attribute> const &variables) {
	size_t stride = 0;
	for (auto variable : variables)
		std::visit(
			vertex_array_variable_visitor{
				[&](size_t width, size_t height, size_t size) {
					stride += size * width * height;
				}
			}, variable
		);

	size_t shift = 0;
	for (auto &variable : variables) {
		attribute_pointer(std::move(buffer), variable, stride, shift);
		std::visit(
			vertex_array_variable_visitor{
				[&](size_t width, size_t height, size_t size) {
					shift += size * width * height;
				}
			}, variable
		);
	}
}

void clap::gl::vertex_array::detail::indexed::draw(connection connection, size_t count, size_t first) {
	this->bind();
	glDrawArrays(gl::detail::convert::to_gl(connection), GLint(first), GLint(count));
	log::message::negligible << "Draw operation on currently bound Vertex Array with " << count << " vertices.";
}

void clap::gl::vertex_array::detail::indexed::draw_indexed(connection connection, size_t count,
														   size_t first, int base_vertex,
														   index_type type) {
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed' cannot be called without a buffer "
			<< "with index data being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	if (base_vertex)
		glDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), GLsizei(count),
								 gl::detail::convert::to_gl(type), (void *) (first * size_t(type)),
								 GLint(base_vertex));
	else
		glDrawElements(gl::detail::convert::to_gl(connection), GLsizei(count),
					   gl::detail::convert::to_gl(type), (void *) (first * size_t(type)));
	log::message::negligible << "Draw operation on currently bound Vertex Array with " << count << " vertices.";
}

void clap::gl::vertex_array::detail::indexed::draw_indexed(connection connection, size_t start,
														   size_t end, size_t count, size_t first,
														   int base_vertex, index_type type) {
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	if (base_vertex)
		glDrawRangeElementsBaseVertex(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
									  GLsizei(count), gl::detail::convert::to_gl(type),
									  (void *) (first * size_t(type)), GLint(base_vertex));
	else
		glDrawRangeElements(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
							GLsizei(count), gl::detail::convert::to_gl(type),
							(void *) (first * size_t(type)));
	log::message::negligible << "Draw operation on currently bound Vertex Array with " << count << " vertices.";
}

void clap::gl::vertex_array::detail::indexed::draw_indirect(connection connection, size_t byte_offset) {
	if (!gl::detail::state::bound(buffer::target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	this->bind();
	glDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_indexed_indirect(connection connection,
																	size_t byte_offset,
																	index_type type) {
	if (!gl::detail::state::bound(buffer::target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_indexed_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed_indirect' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	glDrawElementsIndirect(gl::detail::convert::to_gl(connection),
						   gl::detail::convert::to_gl(type),
						   (void *) (byte_offset));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_multiple(connection connection, size_t drawcount,
															int const *counts, int const *firsts) {
	this->bind();
	glMultiDrawArrays(gl::detail::convert::to_gl(connection), (GLint const *) firsts,
					  (GLint const *) counts, GLsizei(drawcount));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_multiple_indexed(connection connection,
																	size_t drawcount,
																	int const *counts,
																	int const *byte_offsets,
																	index_type type) {
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	glMultiDrawElements(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
						gl::detail::convert::to_gl(type), (void **) (byte_offsets), GLsizei(drawcount));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_multiple_indexed(connection connection,
																	size_t drawcount,
																	int const *counts,
																	int const *byte_offsets,
																	int const *base_vertices,
																	index_type type) {
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	glMultiDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
								  gl::detail::convert::to_gl(type), (void **) (byte_offsets),
								  GLsizei(drawcount), (GLint *) base_vertices);
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_multiple_indirect(connection connection,
																	 size_t drawcount,
																	 size_t byte_offset,
																	 size_t stride) {
	if (!gl::detail::state::bound(buffer::target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_multiple_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	this->bind();
	glMultiDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset),
							  GLsizei(drawcount), GLsizei(stride));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_multiple_indexed_indirect(connection connection,
																			 size_t drawcount,
																			 size_t byte_offset,
																			 size_t stride,
																			 index_type type) {
	if (!gl::detail::state::bound(buffer::target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	glMultiDrawElementsIndirect(gl::detail::convert::to_gl(connection),
								gl::detail::convert::to_gl(type),
								(void *) (byte_offset), GLsizei(drawcount), GLsizei(stride));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

void clap::gl::vertex_array::detail::indexed::draw_instanced(connection connection, size_t count,
															 size_t draw_count, size_t first,
															 int base_instance) {
	this->bind();
	if (base_instance)
		glDrawArraysInstancedBaseInstance(gl::detail::convert::to_gl(connection), GLint(first),
										  GLsizei(count), GLsizei(draw_count), GLuint(base_instance));
	else
		glDrawArraysInstanced(gl::detail::convert::to_gl(connection), GLint(first),
							  GLsizei(count), GLsizei(draw_count));
	log::message::negligible << "Draw operation on currently bound Vertex Array with " << count << " vertices.";
}

void clap::gl::vertex_array::detail::indexed::draw_instanced_indexed(connection connection, size_t count,
																	 size_t draw_count, size_t first,
																	 int base_vertex, int base_instance,
																	 index_type type) {
	if (!gl::detail::state::bound(buffer::target::element_array)) {
		log::warning::critical << "'vertex_array::draw_instanced_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}
	this->bind();
	if (base_instance)
		if (base_vertex)
			glDrawElementsInstancedBaseVertexBaseInstance(gl::detail::convert::to_gl(connection), GLsizei(count),
														  gl::detail::convert::to_gl(type),
														  (void *) (first * size_t(type)),
														  GLsizei(draw_count), GLint(base_vertex),
														  GLuint(base_instance));
		else
			glDrawElementsInstancedBaseInstance(gl::detail::convert::to_gl(connection), GLsizei(count),
												gl::detail::convert::to_gl(type),
												(void *) (first * size_t(type)), GLsizei(draw_count),
												GLuint(base_instance));
	else
		if (base_vertex)
			glDrawElementsInstancedBaseVertex(gl::detail::convert::to_gl(connection), GLsizei(count),
											  gl::detail::convert::to_gl(type),
											  (void *) (first * size_t(type)), GLsizei(draw_count),
											  GLint(base_vertex));
		else
			glDrawElementsInstanced(gl::detail::convert::to_gl(connection), GLsizei(count),
									gl::detail::convert::to_gl(type),
									(void *) (first * size_t(type)), GLsizei(draw_count));
	log::message::negligible << "Draw operation on currently bound Vertex Array.";
}

clap::gl::vertex_array::detail::indexed::operator bool() const {
	return pointer && index < pointer->count;
}
uint32_t clap::gl::vertex_array::detail::indexed::operator*() const { return pointer->ids[index]; }

clap::gl::vertex_array::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds vertex array.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
}