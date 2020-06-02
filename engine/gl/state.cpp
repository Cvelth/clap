#include "state.hpp"
#include "glfw.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "vertex_array.hpp"
#include "../error.hpp"

#include "../../dependencies/glad/glad.h"
#include "../../dependencies/glfw/glfw3.h"

bool engine::gl::state::load() {
	static bool was_loaded = false;
	if (!was_loaded) {
		if (!glfw::is_context_selected) {
			error::critical("An attempt to load GL before a context was created."
							" Call 'make_current' on a window handle first.");
			return false;
		}

		bool out = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		if (!out)
			error::critical("Unable to load GL. 'glad' returns false.");
		was_loaded = true;
		return out;
	} else
		return true;
}

void engine::gl::state::ensure_loaded() { load(); }

std::optional<engine::gl::buffer::detail::indexed> engine::gl::state::bound_buffers[14];
void engine::gl::state::bind(buffer::target const &target, buffer::detail::indexed &&buffer) {
	if (!bound_buffers[size_t(target)] || **bound_buffers[size_t(target)] != *buffer)
		if (*buffer)
			glBindBuffer(detail::convert::to_gl(target), 
						 **(bound_buffers[size_t(target)] = std::move(buffer)));
		else
			error::critical("Buffer passed to 'gl::state::bind' is corrupted.");
}
void engine::gl::state::unbind(buffer::target const &target) {
	glBindBuffer(detail::convert::to_gl(target), 0);
	bound_buffers[size_t(target)] = std::nullopt;
}
std::optional<engine::gl::buffer::detail::indexed> const &engine::gl::state::bound(buffer::target const &target) {
	return bound_buffers[size_t(target)];
}

std::optional<engine::gl::vertex_array::detail::indexed> engine::gl::state::bound_vertex_array;
void engine::gl::state::bind(vertex_array::detail::indexed &&vertex_array) {
	if (!bound_vertex_array || **bound_vertex_array != *vertex_array)
		if (*vertex_array)
			glBindVertexArray(**(bound_vertex_array = std::move(vertex_array)));
		else
			error::critical("Vertex Array passed to 'gl::state::bind' is corrupted.");
}
void engine::gl::state::unbind() {
	glBindVertexArray(0);
	bound_vertex_array = std::nullopt;
}
std::optional<engine::gl::vertex_array::detail::indexed> const &engine::gl::state::bound() {
	return bound_vertex_array;
}

engine::gl::shader::program *engine::gl::state::program_used = nullptr;
void engine::gl::state::use(shader::program *program) {
	if (!program) {
		glUseProgram(0);
		program_used = nullptr;
	} else if (!program_used || program_used->id != program->id) {
		glUseProgram(program->id);
		program_used = program;
	}
}
engine::gl::shader::program *engine::gl::state::being_used() {
	return program_used;
}

void engine::gl::log_last_error() {
	if (auto error = glGetError(); error != GL_NO_ERROR)
		error::warn("GL error logged: " + std::to_string(error));
}
