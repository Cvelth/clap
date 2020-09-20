#include "gl/interface.hpp"
#include "gl/detail/context.hpp"

#include "essential/log.hpp"

clap::gl::detail::object_interface::object_interface() : context(context::current()) {
	if (!context)
		log::warning::critical << "Atempting to create an OpenGL object without a current context.";
}
clap::gl::detail::object_interface::object_interface(object_interface const &another) : context(context::current()) {
	if (context != another.context)
		log::error::critical << "Attempting to copy an object when current context is not the same as the context object was created in.";
}
clap::gl::detail::object_interface::object_interface(object_interface &&another) : context(context::current()) {
	if (context != another.context)
		log::error::critical << "Attempting to move an object when current context is not the same as the context object was created in.";
}
clap::gl::detail::object_interface &clap::gl::detail::object_interface::operator=(object_interface const &another) {
	if (context != another.context)
		log::error::critical << "Attempting to assign an object with different context.";
	return *this;
}
clap::gl::detail::object_interface &clap::gl::detail::object_interface::operator=(object_interface &&another) {
	if (context != another.context)
		log::error::critical << "Attempting to assign an object with different context.";
	return *this;
}

bool clap::gl::detail::verify_context(object_interface const &first, object_interface const &second) {
	if (first.context != second.context) {
		log::warning::major << "Any interfactions between two object created in different contexts are forbidden.";
		return false;
	} else
		return true;
}

bool clap::gl::detail::verify_context(object_interface const &interface) {
	if (context::current() != interface.context) {
		log::warning::major << "Attempting to change an object state "
			"when a context different from the one it was created with is current.";
		return false;
	} else
		return true;
}
