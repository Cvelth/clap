#include "gl/vertex.hpp"

#include "glad/glad.h"

#include "gl/detail/context.hpp"
#include "gl/shader.hpp"
#include "essential/log.hpp"

clap::gl::vertex::buffer::buffer() : size(0) {
	glGenBuffers(1, &id);
	if (id == 0)
		log::warning::critical << "Vertex Buffer Object creation failed.";
	else
		log::message::minor << "A " << *this << " was created.";
}
clap::gl::vertex::buffer::~buffer() {
	if (!verify_context())
		log::error::critical << "Unable to destroy a vertex buffer object when the context it was created in is not current.";

	auto context = gl::detail::context::current();

	// Stop using this buffer (and log a warning) if it's being used on deletion.
	// Make sure no VAOs are bound when the buffer is deleted!

	auto temp_vao = context->bound_vertex_array();
	if (temp_vao)
		glBindVertexArray(0);

	if (id) {
		glDeleteBuffers(1, &id);
		log::message::minor << "A " << *this << " was destroyed.";
	}

	if (temp_vao)
		glBindVertexArray(temp_vao->id);
}

void clap::gl::vertex::buffer::data(void *data, size_t _size, buffer::usage usage, buffer_target target) {
	if (!verify_context()) return;
	size = _size;

	auto guard = bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, data, gl::detail::convert::to_gl(usage));

	if (data)
		log::message::minor << size << " bytes of data were put into a " << *this << " bound to '" << target << "'.";
	else
		log::message::minor << size << " bytes of space were allocated for a " << *this << " bound to '" << target << "'.";

}
void clap::gl::vertex::buffer::subdata(void *data, size_t _size, size_t offset, buffer_target target) {
	if (!verify_context()) return;
	if (offset + _size > size) {
		log::error::major << "Attempting to access data with an index bigger than the size of the buffer.";
		log::info::major << "When calling 'vertex::buffer::subdata' on a " << *this << ".";
	}

	auto guard = bind(target);
	glBufferSubData(gl::detail::convert::to_gl(target), offset, _size, data);

	log::message::minor << _size << " bytes of data were put into a " << *this << " bound to '"
		<< target << "' with offset: " << offset << ".";
}

void clap::gl::vertex::buffer::copy_from(buffer const &another, size_t _size,
										 size_t read_offset, size_t write_offset) {
	if (!verify_context()) return;
	auto source_guard = another.bind(buffer_target::copy_read);
	auto destination_guard = this->bind(buffer_target::copy_write);

	if (read_offset + _size > another.size) {
		log::error::major << "Attempting to access data with an index bigger than the size of the buffer.";
		log::info::major << "When calling 'vertex::buffer::copy_from' on a " << another << ".";
	}
	if (write_offset + _size > another.size) {
		log::error::major << "Attempting to access data with an index bigger than the size of the buffer.";
		log::info::major << "When calling 'vertex::buffer::copy_from' on a " << *this << ".";
	}

	glCopyBufferSubData(gl::detail::convert::to_gl(buffer_target::copy_read),
						gl::detail::convert::to_gl(buffer_target::copy_write),
						read_offset, write_offset, size);

	log::message::minor << size << " bytes of data were copied from a " << another <<
		" with offset " << read_offset << " into a " << *this << " with offset " << write_offset << ".";
}
void clap::gl::vertex::buffer::invalidate(size_t size, size_t offset) {
	if (!verify_context()) return;
	if (offset + size > size) {
		log::error::major << "Attempting to access data with an index bigger than the size of the buffer.";
		log::info::major << "When calling 'vertex::buffer::invalidate' on a " << *this << ".";
	}

	glInvalidateBufferSubData(id, offset, size);
	log::message::minor << size << " bytes of data were invalidated in a " << *this <<
		" with offset: " << offset << ".";
}
void clap::gl::vertex::buffer::invalidate() {
	if (!verify_context()) return;
	glInvalidateBufferData(id);
	log::message::minor << "A " << *this << " was invalidated.";
}


clap::essential::stack<clap::gl::vertex::buffer const *>::iterator clap::gl::vertex::detail::bind_buffer_callable::operator()() {
	auto context = gl::detail::context::current();
	if (!context)
		log::error::critical << "Attempting to use a " << buffer_ref << " without a context being active on the current thread.";
	else if (context != buffer_ref.context)
		log::error::critical << "Attempting to use a " << buffer_ref << " when active context is different from the context object was created with.";
	else if (size_t(target) > size_t(buffer_target::LAST))
		log::error::critical << "Invalid target: " << target << ".";
	else {
		auto out = context->vertex_buffer_stack[size_t(target)].push(&buffer_ref);

		log::message::negligible << "A " << buffer_ref << " was bound to '" << target << "'.";
		log::info::major << "It was added to the top of the stack.";
		glBindBuffer(gl::detail::convert::to_gl(target), buffer_ref.id);

		return out;
	}
}
void clap::gl::vertex::detail::unbind_buffer_callable::operator()(clap::essential::stack<clap::gl::vertex::buffer const *>::iterator iterator) {
	auto context = gl::detail::context::current();
	if (!context)
		log::error::critical << "Attempting to stop using a " << buffer_ref << " without a context being active on the current thread.";
	else if (context != buffer_ref.context)
		log::error::critical << "Attempting to stop using a " << buffer_ref << " when active context is different from the context object was created with.";
	else if (size_t(target) > size_t(buffer_target::LAST))
		log::error::critical << "Invalid target: " << target << ".";
	else if (context->vertex_buffer_stack[size_t(target)].is_front(iterator)) {
		auto active = context->vertex_buffer_stack[size_t(target)].pop();

		log::message::negligible << "A " << *active << " was unbound from '" << target << "'.";
		if (context->vertex_buffer_stack[size_t(target)].empty()) {
			log::info::major << "Stack is empty.";
			glBindBuffer(gl::detail::convert::to_gl(target), 0);
		} else {
			auto reactivated = context->vertex_buffer_stack[size_t(target)].peek();
			log::info::major << "A previously used " << *reactivated << " was rebound, as it's the next element on the stack.";
			glBindBuffer(gl::detail::convert::to_gl(target), reactivated->id);
		}
	} else {
		log::message::negligible << "Removing a " << buffer_ref << " from bound vertex::buffer[" << target << "] stack.";
		log::info::major << "This doesn't affect any currently bound buffer in any way.";
		context->vertex_buffer_stack[size_t(target)].erase(iterator);
	}
}

clap::gl::vertex::array::array() {
	glGenVertexArrays(1, &id);
	if (id == 0)
		log::warning::critical << "Vertex Array Object creation failed.";
	else
		log::message::minor << "A " << *this << " was created.";
}
clap::gl::vertex::array::~array() {
	if (!verify_context())
		log::error::critical << "Unable to destroy a vertex array object when the context it was created in is not current.";

	// Stop using this array (and log a warning) if it's being used on deletion.

	if (id) {
		glDeleteVertexArrays(1, &id);
		log::message::minor << "A " << *this << " was destroyed.";
	}
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

void clap::gl::vertex::array::attribute_pointer(vertex::buffer &buffer, shader::variable::attribute const &variable,
												size_t stride, size_t shift) {
	if (!verify_context()) return;

	auto array_guard = this->bind();
	auto buffer_guard = buffer.bind();

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
	log::message::minor << "A " << variable << " uses " << buffer << " as inputs.";
	log::info::minor << "State is stored in: " << *this << ".";
	log::info::minor << "Stride: " << stride << ".";
	log::info::minor << "Shift: " << shift << ".";
}

void clap::gl::vertex::array::attribute_divisor(shader::variable::attribute const &variable, size_t divisor) {
	if (!verify_context()) return;

	auto guard = this->bind();
	std::visit(
		vertex_array_variable_visitor{
			[&](size_t width, size_t height, size_t size) {
				for (size_t i = 0; i < height; i++)
					glVertexAttribDivisor(GLuint(variable.location + i), GLuint(divisor));
			}
		}, variable
	);
	log::message::minor << "A " << variable << " uses a divisor: " << divisor << ".";
	log::info::minor << "State is stored in: " << *this << ".";
}

void clap::gl::vertex::array::attribute_pointer(vertex::buffer &buffer, shader::variable::attribute const &variable,
												size_t stride, size_t shift, size_t divisor) {
	attribute_pointer(buffer, variable, stride, shift);
	attribute_divisor(variable, divisor);
}

void clap::gl::vertex::array::attribute_pointer(vertex::buffer &buffer,
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
		attribute_pointer(buffer, variable, stride, shift);
		std::visit(
			vertex_array_variable_visitor{
				[&](size_t width, size_t height, size_t size) {
					shift += size * width * height;
				}
			}, variable
		);
	}
}

void clap::gl::vertex::array::draw(connection connection, size_t count, size_t first) {
	if (!verify_context()) return;

	auto guard = this->bind();
	glDrawArrays(gl::detail::convert::to_gl(connection), GLint(first), GLint(count));
	log::message::negligible << "Draw operation on " << *this << " with " << count << " vertices.";
}

void clap::gl::vertex::array::draw_indexed(connection connection, size_t count, size_t first,
										   int base_vertex, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed' cannot be called without a buffer "
			"with index data being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	if (base_vertex)
		glDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), GLsizei(count),
								 gl::detail::convert::to_gl(type), (void *) (first * size_t(type)),
								 GLint(base_vertex));
	else
		glDrawElements(gl::detail::convert::to_gl(connection), GLsizei(count),
					   gl::detail::convert::to_gl(type), (void *) (first * size_t(type)));
	log::message::negligible << "Draw operation on " << *this << " with " << count << " vertices.";
}

void clap::gl::vertex::array::draw_indexed(connection connection, size_t start, size_t end, size_t count,
										   size_t first, int base_vertex, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	if (base_vertex)
		glDrawRangeElementsBaseVertex(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
									  GLsizei(count), gl::detail::convert::to_gl(type),
									  (void *) (first * size_t(type)), GLint(base_vertex));
	else
		glDrawRangeElements(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
							GLsizei(count), gl::detail::convert::to_gl(type),
							(void *) (first * size_t(type)));
	log::message::negligible << "Draw operation on " << *this << " with " << count << " vertices.";
}

void clap::gl::vertex::array::draw_indirect(connection connection, size_t byte_offset) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}

	auto guard = this->bind();
	glDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_indexed_indirect(connection connection, size_t byte_offset, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_indexed_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_indexed_indirect' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	glDrawElementsIndirect(gl::detail::convert::to_gl(connection),
						   gl::detail::convert::to_gl(type),
						   (void *) (byte_offset));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_multiple(connection connection, size_t drawcount,
											int const *counts, int const *firsts) {
	if (!verify_context()) return;

	auto guard = this->bind();
	glMultiDrawArrays(gl::detail::convert::to_gl(connection), (GLint const *) firsts,
					  (GLint const *) counts, GLsizei(drawcount));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_multiple_indexed(connection connection, size_t drawcount, int const *counts,
													int const *byte_offsets, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	glMultiDrawElements(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
						gl::detail::convert::to_gl(type), (void **) (byte_offsets), GLsizei(drawcount));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_multiple_indexed(connection connection, size_t drawcount,
													int const *counts, int const *byte_offsets,
													int const *base_vertices, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	glMultiDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
								  gl::detail::convert::to_gl(type), (void **) (byte_offsets),
								  GLsizei(drawcount), (GLint *) base_vertices);
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_multiple_indirect(connection connection, size_t drawcount,
													 size_t byte_offset, size_t stride) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_multiple_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}

	auto guard = this->bind();
	glMultiDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset),
							  GLsizei(drawcount), GLsizei(stride));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_multiple_indexed_indirect(connection connection, size_t drawcount, size_t byte_offset,
															 size_t stride, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::indirect_draw)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with draw data "
			"being bound to 'buffer::target::indirect_draw'.";
		return;
	}
	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
	glMultiDrawElementsIndirect(gl::detail::convert::to_gl(connection),
								gl::detail::convert::to_gl(type),
								(void *) (byte_offset), GLsizei(drawcount), GLsizei(stride));
	log::message::negligible << "Draw operation on " << *this << ".";
}

void clap::gl::vertex::array::draw_instanced(connection connection, size_t count, size_t draw_count,
											 size_t first, int base_instance) {
	if (!verify_context()) return;

	auto guard = this->bind();
	if (base_instance)
		glDrawArraysInstancedBaseInstance(gl::detail::convert::to_gl(connection), GLint(first),
										  GLsizei(count), GLsizei(draw_count), GLuint(base_instance));
	else
		glDrawArraysInstanced(gl::detail::convert::to_gl(connection), GLint(first),
							  GLsizei(count), GLsizei(draw_count));
	log::message::negligible << "Draw operation on " << *this << " with " << count << " vertices.";
}

void clap::gl::vertex::array::draw_instanced_indexed(connection connection, size_t count, size_t draw_count, size_t first,
													 int base_vertex, int base_instance, index_type type) {
	if (!verify_context()) return;

	if (!gl::detail::context::current()->bound_vertex_buffer(vertex::buffer_target::element_array)) {
		log::warning::critical << "'vertex_array::draw_instanced_indexed' cannot be called without a buffer with index data "
			"being bound to 'buffer::target::element_array'.";
		return;
	}

	auto guard = this->bind();
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
	log::message::negligible << "Draw operation on " << *this << ".";
}

clap::essential::stack<clap::gl::vertex::array const *>::iterator clap::gl::vertex::detail::bind_array_callable::operator()() {
	auto context = gl::detail::context::current();
	if (!context)
		log::error::critical << "Attempting to use a " << array_ref << " without a context being active on the current thread.";
	else if (context != array_ref.context)
		log::error::critical << "Attempting to use a " << array_ref << " when active context is different from the context object was created with.";
	else {
		auto out = context->vertex_array_stack.push(&array_ref);

		log::message::negligible << "A " << array_ref << " was bound.";
		log::info::major << "It was added to the top of the stack.";
		glBindVertexArray(array_ref.id);

		return out;
	}
}
void clap::gl::vertex::detail::unbind_array_callable::operator()(clap::essential::stack<clap::gl::vertex::array const *>::iterator iterator) {
	auto context = gl::detail::context::current();
	if (!context)
		log::error::critical << "Attempting to stop using a " << array_ref << " without a context being active on the current thread.";
	else if (context != array_ref.context)
		log::error::critical << "Attempting to stop using a " << array_ref << " when active context is different from the context object was created with.";
	else if (context->vertex_array_stack.is_front(iterator)) {
		auto active = context->vertex_array_stack.pop();

		log::message::negligible << "A " << *active << " was unbound.";
		if (context->vertex_array_stack.empty()) {
			log::info::major << "Stack is empty.";
			glBindVertexArray(0);
		} else {
			auto reactivated = context->vertex_array_stack.peek();
			log::info::major << "A previously used " << *reactivated << " was rebound, as it's the next element on the stack.";
			glBindVertexArray(reactivated->id);
		}
	} else {
		log::message::negligible << "Removing a " << array_ref << " from bound vertex::array stack.";
		log::info::major << "This doesn't affect any currently bound array in any way.";
		context->vertex_array_stack.erase(iterator);
	}
}