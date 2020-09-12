#include "gl/vertex_array.hpp"

#include "glad/glad.h"

#include "gl/detail/state.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "essential/log.hpp"

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

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																shader::detail::variable const &variable,
																size_t stride, size_t shift) {
	if (variable.storage != shader::detail::variable_type_t::storage::attribute) {
		log::warning::critical << "Cannot pass a non-attribute variable to 'vertex_array::attribute_pointer'";
		return;
	}
	if (variable.type.structure != shader::detail::variable_type_t::structure::data) {
		log::warning::critical << "Cannot pass a non-data variable to 'vertex_array::attribute_pointer'";
		return;
	}

	this->bind();
	buffer.bind();
	for (size_t i = 0; i < variable.type.dimentions.x; i++) {
		glVertexAttribPointer(GLuint(variable.location + i),
							  GLint(variable.type.dimentions.x * variable.type.dimentions.y),
							  gl::detail::convert::to_gl(variable.type.datatype),
							  GL_FALSE, GLsizei(stride * variable.datatype_size()),
							  (const void *) (shift * variable.datatype_size()));
		glEnableVertexAttribArray(GLuint(variable.location + i));

		std::string index_string = variable.type.dimentions.x != 1 ? ('[' + std::to_string(i) + ']') : "";
		log::message::minor << "Variable '" << variable.name << index_string
			<< "' uses data stored in buffer currently bound to '"
			<< buffer::target::array << "'.";
	}
}

void clap::gl::vertex_array::detail::indexed::attribute_divisor(shader::detail::variable const &variable,
																size_t divisor) {
	if (variable.storage != shader::detail::variable_type_t::storage::attribute) {
		log::warning::critical << "Cannot pass a non-attribute variable to 'vertex_array::attribute_divisor'";
		return;
	}
	if (variable.type.structure != shader::detail::variable_type_t::structure::data) {
		log::warning::critical << "Cannot pass a non-data variable to 'vertex_array::attribute_divisor'";
		return;
	}

	this->bind();
	for (int i = 0; i < variable.type.dimentions.x; i++) {
		glVertexAttribDivisor(variable.location, GLuint(divisor));

		std::string index_string = variable.type.dimentions.x != 1 ? ('[' + std::to_string(i) + ']') : "";
		log::message::minor << "Variable '" << variable.name << index_string << "' uses a divisor: "
			<< divisor << ".";
	}
}

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																shader::detail::variable const &variable,
																size_t stride, size_t shift,
																size_t divisor) {
	attribute_pointer(std::move(buffer), variable, stride, shift);
	attribute_divisor(variable, divisor);
}

void clap::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																shader::variables const &variables) {
	size_t stride = 0;
	for (auto variable : variables)
		stride += variable.second.size();
	size_t shift = 0;
	for (auto &variable : variables) {
		attribute_pointer(std::move(buffer), variable.second, stride, shift);
		shift += variable.second.size();
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