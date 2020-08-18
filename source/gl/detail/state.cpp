#include "gl/detail/state.hpp"

#include "glad/gl.h"
#include "glfw/glfw3.h"

#include "window/detail/glfw.hpp"
#include "gl/shader.hpp"
#include "essential/log.hpp"

bool clap::gl::detail::state::load() {
	static bool was_loaded = false;
	if (!was_loaded) {
		if (!clap::detail::glfw::is_context_selected) {
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

void clap::gl::detail::state::ensure_loaded() { load(); }

std::optional<clap::gl::buffer::detail::indexed> clap::gl::detail::state::bound_buffers[buffer_target_count];
void clap::gl::detail::state::bind(buffer::target const &target, buffer::detail::indexed &&buffer) {
	if (!bound_buffers[size_t(target)] || **bound_buffers[size_t(target)] != *buffer)
		if (*buffer) {
			log::message::minor << "A new buffer is bound to '" << target << "'.";
			glBindBuffer(detail::convert::to_gl(target),
						 **(bound_buffers[size_t(target)] = std::move(buffer)));
		} else
			log::warning::critical << "Buffer passed to 'gl::state::bind' is corrupted.";
}
std::optional<clap::gl::buffer::detail::indexed> const clap::gl::detail::state::unbind(buffer::target const &target) {
	glBindBuffer(detail::convert::to_gl(target), 0);
	auto out = std::move(bound_buffers[size_t(target)]);
	bound_buffers[size_t(target)] = std::nullopt;
	log::message::minor << "A buffer is no longer bound to '" << target << "'.";
	return std::move(out);
}
std::optional<clap::gl::buffer::detail::indexed> const &clap::gl::detail::state::bound(buffer::target const &target) {
	return bound_buffers[size_t(target)];
}
std::optional<clap::gl::buffer::target> clap::gl::detail::state::is_bound(clap::gl::buffer::detail::indexed const &buffer) {
	for (size_t i = 0; i < buffer_target_count; i++)
		if (bound_buffers[i] && **bound_buffers[i] == *buffer)
			return clap::gl::buffer::target(i);
	return std::nullopt;
}

std::optional<clap::gl::vertex_array::detail::indexed> clap::gl::detail::state::bound_vertex_array;
void clap::gl::detail::state::bind(vertex_array::detail::indexed &&vertex_array) {
	if (!bound_vertex_array || **bound_vertex_array != *vertex_array)
		if (*vertex_array) {
			log::message::minor << "A new vertex array is bound.";
			glBindVertexArray(**(bound_vertex_array = std::move(vertex_array)));
		} else
			log::warning::critical << "Vertex Array passed to 'gl::state::bind' is corrupted.";
}
std::optional<clap::gl::vertex_array::detail::indexed> const clap::gl::detail::state::unbind() {
	glBindVertexArray(0);
	auto out = std::move(bound_vertex_array);
	bound_vertex_array = std::nullopt;
	log::message::minor << "A vertex_array is no longer bound.";
	return std::move(out);
}
std::optional<clap::gl::vertex_array::detail::indexed> const &clap::gl::detail::state::bound() {
	return bound_vertex_array;
}
bool clap::gl::detail::state::is_bound(vertex_array::detail::indexed const &vertex_array) {
	return bound_vertex_array && **bound_vertex_array == *vertex_array;
}

clap::gl::shader::program *clap::gl::detail::state::program_used = nullptr;
void clap::gl::detail::state::use(shader::program *program) {
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
clap::gl::shader::program *clap::gl::detail::state::being_used() {
	return program_used;
}
bool clap::gl::detail::state::is_used(shader::program *program) {
	return program_used && program_used->id == program->id;
}

std::optional<clap::gl::texture::detail::indexed> clap::gl::detail::state::bound_textures[texture_target_count];
void clap::gl::detail::state::bind(texture::target const &target, texture::detail::indexed &&texture) {
	if (!bound_textures[size_t(target)] || **bound_textures[size_t(target)] != *texture)
		if (*texture) {
			log::message::minor << "A new texture is bound to '" << target << "'.";
			glBindTexture(detail::convert::to_gl(target),
						 **(bound_textures[size_t(target)] = std::move(texture)));
		} else
			log::warning::critical << "A texture object passed to 'gl::state::bind' is corrupted.";
}
std::optional<clap::gl::texture::detail::indexed> const clap::gl::detail::state::unbind(texture::target const &target) {
	glBindTexture(detail::convert::to_gl(target), 0);
	auto out = std::move(bound_textures[size_t(target)]);
	bound_textures[size_t(target)] = std::nullopt;
	log::message::minor << "A texture objec is no longer bound to '" << target << "'.";
	return std::move(out);
}
std::optional<clap::gl::texture::detail::indexed> const &clap::gl::detail::state::bound(texture::target const &target) {
	return bound_textures[size_t(target)];
}
std::optional<clap::gl::texture::target> clap::gl::detail::state::is_bound(clap::gl::texture::detail::indexed const &texture) {
	for (size_t i = 0; i < texture_target_count; i++)
		if (bound_textures[i] && **bound_textures[i] == *texture)
			return clap::gl::texture::target(i);
	return std::nullopt;
}