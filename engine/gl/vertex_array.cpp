#include "vertex_array.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "state.hpp"
#include "../error.hpp"

#include "../../dependencies/glad/glad.h"

engine::gl::vertex_array::multiple::multiple(size_t count)
	: count(count) {
	gl::state::ensure_loaded();

	ids = new uint32_t[count];
	glGenBuffers(GLsizei(count), ids);
}
engine::gl::vertex_array::multiple::~multiple() {
	if (count) delete[] ids;
}

engine::gl::vertex_array::detail::indexed engine::gl::vertex_array::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

engine::gl::vertex_array::multiple::multiple(size_t count, uint32_t *ids) : count(count), ids(ids) {
	gl::state::ensure_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsBuffer(ids[i]))
			error::critical("Unable to perform a buffer move operation. "
							"Passed buffer seems to be corrupted.");
}

void engine::gl::vertex_array::detail::indexed::bind() {
	state::bind(std::move(*this));
}

void engine::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																  shader::variable const &variable,
																  size_t stride, size_t shift) {
	buffer.bind();
	this->bind();
	for (int i = 0; i < variable.dimentions.x; i++) {
		glVertexAttribPointer(variable.location + i, GLint(variable.dimentions.x * variable.dimentions.y),
							  gl::detail::convert::to_gl(variable.datatype),
							  GL_FALSE, GLsizei(stride), (const void *) shift);
		glEnableVertexAttribArray(variable.location + i);
	}
}

void engine::gl::vertex_array::detail::indexed::attribute_divisor(shader::variable const &variable,
																  size_t divisor) {
	this->bind();
	for (int i = 0; i < variable.dimentions.x; i++)
		glVertexAttribDivisor(variable.location, GLuint(divisor));
}

void engine::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																  shader::variable const &variable,
																  size_t stride, size_t shift,
																  size_t divisor) {
	attribute_pointer(std::move(buffer), variable, stride, shift);
	attribute_divisor(variable, divisor);
}

void engine::gl::vertex_array::detail::indexed::attribute_pointer(buffer::detail::indexed &&buffer,
																  std::set<shader::variable> const &variables) {
	size_t stride = 0;
	for (auto variable : variables)
		stride += variable.size();
	size_t shift = 0;
	for (auto &it : variables) {
		attribute_pointer(std::move(buffer), it, stride, shift);
		shift += it.size();
	}
}

void engine::gl::vertex_array::detail::indexed::draw(connection connection, size_t count, size_t first) {
	bind();
	glDrawArrays(gl::detail::convert::to_gl(connection), GLint(first), GLint(count));
}

void engine::gl::vertex_array::detail::indexed::draw_indexed(connection connection, size_t count,
															 size_t first, int base_vertex,
															 index_type type) {
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_indexed' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	if (base_vertex)
		glDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), GLsizei(count),
								 gl::detail::convert::to_gl(type), (void *) (first * size_t(type)),
								 GLint(base_vertex));
	else
		glDrawElements(gl::detail::convert::to_gl(connection), GLsizei(count),
					   gl::detail::convert::to_gl(type), (void *) (first * size_t(type)));
}

void engine::gl::vertex_array::detail::indexed::draw_indexed(connection connection, size_t start,
															 size_t end, size_t count, size_t first,
															 int base_vertex, index_type type) {
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_indexed' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	if (base_vertex)
		glDrawRangeElementsBaseVertex(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
									  GLsizei(count), gl::detail::convert::to_gl(type),
									  (void *) (first * size_t(type)), GLint(base_vertex));
	else
		glDrawRangeElements(gl::detail::convert::to_gl(connection), GLuint(start), GLuint(end),
							GLsizei(count), gl::detail::convert::to_gl(type),
							(void *) (first * size_t(type)));
}

void engine::gl::vertex_array::detail::indexed::draw_indirect(connection connection, size_t byte_offset) {
	if (!state::bound(buffer::target::indirect_draw))
		error::critical("'vertex_array::draw_indirect' cannot be called without a buffer with draw data "
						"being bound to 'buffer::target::indirect_draw'.");
	bind();
	glDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset));
}

void engine::gl::vertex_array::detail::indexed::draw_indexed_indirect(connection connection,
																	  size_t byte_offset,
																	  index_type type) {
	if (!state::bound(buffer::target::indirect_draw))
		error::critical("'vertex_array::draw_indexed_indirect' cannot be called without a buffer with draw data "
						"being bound to 'buffer::target::indirect_draw'.");
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_indexed_indirect' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	glDrawElementsIndirect(gl::detail::convert::to_gl(connection),
						   gl::detail::convert::to_gl(type),
						   (void *) (byte_offset));
}

void engine::gl::vertex_array::detail::indexed::draw_multiple(connection connection, size_t drawcount,
															  int const *counts, int const *firsts) {
	bind();
	glMultiDrawArrays(gl::detail::convert::to_gl(connection), (GLint const *) firsts,
					  (GLint const *) counts, GLsizei(drawcount));
}

void engine::gl::vertex_array::detail::indexed::draw_multiple_indexed(connection connection,
																	  size_t drawcount,
																	  int const *counts,
																	  int const *byte_offsets,
																	  index_type type) {
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	glMultiDrawElements(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
						gl::detail::convert::to_gl(type), (void **) (byte_offsets), GLsizei(drawcount));
}

void engine::gl::vertex_array::detail::indexed::draw_multiple_indexed(connection connection,
																	  size_t drawcount,
																	  int const *counts,
																	  int const *byte_offsets,
																	  int const *base_vertices,
																	  index_type type) {
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_multiple_indexed' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	glMultiDrawElementsBaseVertex(gl::detail::convert::to_gl(connection), (GLsizei *) counts,
								  gl::detail::convert::to_gl(type), (void **) (byte_offsets),
								  GLsizei(drawcount), (GLint *) base_vertices);
}

void engine::gl::vertex_array::detail::indexed::draw_multiple_indirect(connection connection, 
																	   size_t drawcount, 
																	   size_t byte_offset, 
																	   size_t stride) {
	if (!state::bound(buffer::target::indirect_draw))
		error::critical("'vertex_array::draw_multiple_indirect' cannot be called without a buffer with draw data "
						"being bound to 'buffer::target::indirect_draw'.");
	bind();
	glMultiDrawArraysIndirect(gl::detail::convert::to_gl(connection), (void *) (byte_offset), 
							  GLsizei(drawcount), GLsizei(stride));
}

void engine::gl::vertex_array::detail::indexed::draw_multiple_indexed_indirect(connection connection, 
																			   size_t drawcount, 
																			   size_t byte_offset, 
																			   size_t stride, 
																			   index_type type) {
	if (!state::bound(buffer::target::indirect_draw))
		error::critical("'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with draw data "
						"being bound to 'buffer::target::indirect_draw'.");
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_multiple_indexed_indirect' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
	glMultiDrawElementsIndirect(gl::detail::convert::to_gl(connection), 
								gl::detail::convert::to_gl(type),
								(void *) (byte_offset), GLsizei(drawcount), GLsizei(stride));
}

void engine::gl::vertex_array::detail::indexed::draw_instanced(connection connection, size_t count, 
															   size_t draw_count, size_t first, 
															   int base_instance) {
	bind();
	if (base_instance)
		glDrawArraysInstancedBaseInstance(gl::detail::convert::to_gl(connection), GLint(first),
										  GLsizei(count), GLsizei(draw_count), GLuint(base_instance));
	else
		glDrawArraysInstanced(gl::detail::convert::to_gl(connection), GLint(first), 
							  GLsizei(count), GLsizei(draw_count));
}

void engine::gl::vertex_array::detail::indexed::draw_instanced_indexed(connection connection, size_t count,
																	   size_t draw_count, size_t first,
																	   int base_vertex, int base_instance,
																	   index_type type) {
	if (!state::bound(buffer::target::element_array))
		error::critical("'vertex_array::draw_instanced_indexed' cannot be called without a buffer with index data "
						"being bound to 'buffer::target::element_array'.");
	bind();
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
}

GLenum engine::gl::detail::convert::to_gl(engine::gl::vertex_array::connection v) {
	switch (v) {
		case engine::gl::vertex_array::connection::points: return GL_POINTS;
		case engine::gl::vertex_array::connection::lines: return GL_LINES;
		case engine::gl::vertex_array::connection::lines_w_adjacency: return GL_LINES_ADJACENCY;
		case engine::gl::vertex_array::connection::line_strip: return GL_LINE_STRIP;
		case engine::gl::vertex_array::connection::line_strip_w_adjacency: return GL_LINE_STRIP_ADJACENCY;
		case engine::gl::vertex_array::connection::line_loop: return GL_LINE_LOOP;
		case engine::gl::vertex_array::connection::triangles: return GL_TRIANGLES;
		case engine::gl::vertex_array::connection::triangles_w_adjacency: return GL_TRIANGLES_ADJACENCY;
		case engine::gl::vertex_array::connection::triangle_strip: return GL_TRIANGLE_STRIP;
		case engine::gl::vertex_array::connection::triangle_strip_w_adjacency: return GL_TRIANGLE_STRIP_ADJACENCY;;
		case engine::gl::vertex_array::connection::triangle_fan: return GL_TRIANGLE_FAN;
		case engine::gl::vertex_array::connection::patches: return GL_PATCHES;
	}
	engine::error::critical("Unsupported enum value.");
}

engine::gl::vertex_array::connection engine::gl::detail::convert::to_connection_type(GLenum v) {
	switch (v) {
		case GL_POINTS:						  return engine::gl::vertex_array::connection::points;
		case GL_LINES:						  return engine::gl::vertex_array::connection::lines;
		case GL_LINES_ADJACENCY:			  return engine::gl::vertex_array::connection::lines_w_adjacency;
		case GL_LINE_STRIP:					  return engine::gl::vertex_array::connection::line_strip;
		case GL_LINE_STRIP_ADJACENCY:		  return engine::gl::vertex_array::connection::line_strip_w_adjacency;
		case GL_LINE_LOOP:					  return engine::gl::vertex_array::connection::line_loop;
		case GL_TRIANGLES:					  return engine::gl::vertex_array::connection::triangles;
		case GL_TRIANGLES_ADJACENCY:		  return engine::gl::vertex_array::connection::triangles_w_adjacency;
		case GL_TRIANGLE_STRIP:				  return engine::gl::vertex_array::connection::triangle_strip;
		case GL_TRIANGLE_STRIP_ADJACENCY:	  return engine::gl::vertex_array::connection::triangle_strip_w_adjacency;
		case GL_TRIANGLE_FAN:				  return engine::gl::vertex_array::connection::triangle_fan;
		case GL_PATCHES:					  return engine::gl::vertex_array::connection::patches;
	}
	engine::error::critical("Unsupported enum value.");
}

GLenum engine::gl::detail::convert::to_gl(engine::gl::vertex_array::index_type v) {
	switch (v) {
		case engine::gl::vertex_array::index_type::unsigned_byte: return GL_UNSIGNED_BYTE;
		case engine::gl::vertex_array::index_type::unsigned_short: return GL_UNSIGNED_SHORT;
		case engine::gl::vertex_array::index_type::unsigned_int: return GL_UNSIGNED_INT;
	}
	engine::error::critical("Unsupported enum value.");
}

engine::gl::vertex_array::index_type engine::gl::detail::convert::to_index_type(GLenum v) {
	switch (v) {
		case GL_UNSIGNED_BYTE: return engine::gl::vertex_array::index_type::unsigned_byte;
		case GL_UNSIGNED_SHORT: return engine::gl::vertex_array::index_type::unsigned_short;
		case GL_UNSIGNED_INT: return engine::gl::vertex_array::index_type::unsigned_int;
	}
	engine::error::critical("Unsupported enum value.");
}

engine::gl::vertex_array::detail::indexed::operator bool() const { 
	return pointer && index < pointer->count; 
}
uint32_t engine::gl::vertex_array::detail::indexed::operator*() const { return pointer->ids[index]; }

engine::gl::vertex_array::detail::indexed::indexed(multiple *pointer, size_t index)
												: pointer(pointer), index(index) {
	if (!*this)
		error::critical("Attempting to access an out of bounds buffer.");
}