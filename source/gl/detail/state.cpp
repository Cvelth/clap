#include "gl/detail/state.hpp"

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "window/detail/glfw.hpp"
#include "gl/buffer.hpp"
#include "gl/shader.hpp"
#include "gl/vertex_array.hpp"
#include "essential/log.hpp"

bool engine::gl::detail::state::load() {
	static bool was_loaded = false;
	if (!was_loaded) {
		if (!engine::detail::glfw::is_context_selected) {
			log::error::critical << "An attempt to load GL before a context was created. "
				"Call 'make_current' on a window handle first.";
			return false;
		}

		bool out = gladLoadGL((GLADloadfunc) glfwGetProcAddress);
		if (!out)
			log::error::critical << "Unable to load GL. 'glad' returns false.";
		was_loaded = true;

		log::message::critical << "GL was loaded using 'glad'.";
		log::info::critical << "GL Version: " << glGetString(GL_VERSION);
		log::info::negligible << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
		log::info::negligible << "Vendor: " << glGetString(GL_VENDOR);
		log::info::major << "Renderer: " << glGetString(GL_RENDERER);

		return out;
	} else
		return true;
}

void engine::gl::detail::state::ensure_loaded() { load(); }

std::optional<engine::gl::buffer::detail::indexed> engine::gl::detail::state::bound_buffers[buffer_target_count];
void engine::gl::detail::state::bind(buffer::target const &target, buffer::detail::indexed &&buffer) {
	if (!bound_buffers[size_t(target)] || **bound_buffers[size_t(target)] != *buffer)
		if (*buffer) {
			log::message::minor << "A new buffer is bound to '" << target << "'.";
			glBindBuffer(detail::convert::to_gl(target),
						 **(bound_buffers[size_t(target)] = std::move(buffer)));
		} else
			log::warning::critical << "Buffer passed to 'gl::state::bind' is corrupted.";
}
std::optional<engine::gl::buffer::detail::indexed> const engine::gl::detail::state::unbind(buffer::target const &target) {
	glBindBuffer(detail::convert::to_gl(target), 0);
	auto out = std::move(bound_buffers[size_t(target)]);
	bound_buffers[size_t(target)] = std::nullopt;
	log::message::minor << "A buffer is no longer bound to '" << target << "'.";
	return std::move(out);
}
std::optional<engine::gl::buffer::detail::indexed> const &engine::gl::detail::state::bound(buffer::target const &target) {
	return bound_buffers[size_t(target)];
}
std::optional<engine::gl::buffer::target> engine::gl::detail::state::is_bound(engine::gl::buffer::detail::indexed const &buffer) {
	for (size_t i = 0; i < buffer_target_count; i++)
		if (bound_buffers[i] && **bound_buffers[i] == *buffer)
			return engine::gl::buffer::target(i);
	return std::nullopt;
}

std::optional<engine::gl::vertex_array::detail::indexed> engine::gl::detail::state::bound_vertex_array;
void engine::gl::detail::state::bind(vertex_array::detail::indexed &&vertex_array) {
	if (!bound_vertex_array || **bound_vertex_array != *vertex_array)
		if (*vertex_array) {
			log::message::minor << "A new vertex array is bound.";
			glBindVertexArray(**(bound_vertex_array = std::move(vertex_array)));
		} else
			log::warning::critical << "Vertex Array passed to 'gl::state::bind' is corrupted.";
}
std::optional<engine::gl::vertex_array::detail::indexed> const engine::gl::detail::state::unbind() {
	glBindVertexArray(0);
	auto out = std::move(bound_vertex_array);
	bound_vertex_array = std::nullopt;
	log::message::minor << "A vertex_array is no longer bound.";
	return std::move(out);
}
std::optional<engine::gl::vertex_array::detail::indexed> const &engine::gl::detail::state::bound() {
	return bound_vertex_array;
}
bool engine::gl::detail::state::is_bound(vertex_array::detail::indexed const &vertex_array) {
	return bound_vertex_array && **bound_vertex_array == *vertex_array;
}

engine::gl::shader::program *engine::gl::detail::state::program_used = nullptr;
void engine::gl::detail::state::use(shader::program *program) {
	if (!program) {
		glUseProgram(0);
		program_used = nullptr;
		log::message::minor << "A shader::program is no longer used.";
	} else if (!program_used || program_used->id != program->id) {
		glUseProgram(program->id);
		program_used = program;
		log::message::minor << "A new shader::program is used.";
	} 
}
engine::gl::shader::program *engine::gl::detail::state::being_used() {
	return program_used;
}
bool engine::gl::detail::state::is_used(shader::program *program) {
	return program_used && program_used->id == program->id;
}
