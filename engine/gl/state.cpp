#include "state.hpp"
#include "glfw.hpp"
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

void engine::gl::state::check_load() { load(); }

engine::gl::buffer::detail::indexed *engine::gl::state::bound_buffers[14];
void engine::gl::state::bind(buffer::target const &target, buffer::detail::indexed *buffer) {
	if (buffer && *buffer) {
		glBindBuffer(detail::convert::to_gl(target), **buffer);
		bound_buffers[size_t(target)] = buffer;
	} else {
		glBindBuffer(detail::convert::to_gl(target), 0);
		bound_buffers[size_t(target)] = nullptr;
	}
}
engine::gl::buffer::detail::indexed *engine::gl::state::bound(buffer::target const &target) {
	return bound_buffers[size_t(target)];
}