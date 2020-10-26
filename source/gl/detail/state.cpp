#include "gl/detail/state.hpp"

#include "glad/glad.h"

#include "gl/shader.hpp"
#include "essential/log.hpp"

bool clap::gl::detail::state::was_loaded = false;
void clap::gl::detail::state::verify_loaded() { 
	if (!was_loaded)
		clap::log::error::critical << "An attempt to use OpenGL before it was loaded.";
}