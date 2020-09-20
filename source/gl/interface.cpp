#include "gl/interface.hpp"
#include "gl/detail/context.hpp"

#include "essential/log.hpp"

clap::gl::object_interface::object_interface() : context(detail::context::current()) {
	if (!context)
		clap::log::warning::critical << "Atempting to create an OpenGL object without a current context.";
}
