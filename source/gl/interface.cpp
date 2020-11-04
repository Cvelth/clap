#include "gl/interface.hpp"
#include "gl/detail/context.hpp"

#include "essential/log.hpp"

clap::gl::detail::object_interface::object_interface() : context_ptr(*context::current()) {
	if (!context_ptr)
		log::warning::critical << "Atempting to create an OpenGL object without a current context.";
}
clap::gl::detail::object_interface::object_interface(object_interface const &another) : context_ptr(*context::current()) {
	if (context_ptr != another.context_ptr)
		log::error::critical << "Attempting to copy an object when current context is not the same as the context object was created in.";
}
clap::gl::detail::object_interface::object_interface(object_interface &&another) : context_ptr(*context::current()) {
	if (context_ptr != another.context_ptr)
		log::error::critical << "Attempting to move an object when current context is not the same as the context object was created in.";
}
clap::gl::detail::object_interface &clap::gl::detail::object_interface::operator=(object_interface const &another) {
	if (context_ptr != another.context_ptr)
		log::error::critical << "Attempting to assign an object with different context.";
	return *this;
}
clap::gl::detail::object_interface &clap::gl::detail::object_interface::operator=(object_interface &&another) {
	if (context_ptr != another.context_ptr)
		log::error::critical << "Attempting to assign an object with different context.";
	return *this;
}

clap::gl::detail::context::access_guard clap::gl::detail::object_interface::access_context_s() {
	auto guard = context::current();
	if (!guard)
		log::warning::critical << "Attempting to access context on a thread where none is activated.";
	return guard;
}
clap::gl::detail::context::access_guard clap::gl::detail::object_interface::access_context() const {
	auto guard = access_context_s();
	if (*guard != context_ptr)
		log::warning::critical << "Attempting to change an " << *this << " state "
		"when a context different from the one it was created with is current.";
	return guard;
}
clap::gl::detail::context::access_guard clap::gl::detail::object_interface::access_context(object_interface const &another) const {
	auto guard = access_context();
	if (*guard != another.context_ptr) {
		log::warning::critical << "Any interfactions between two object created in different contexts are forbidden.";
		log::info::critical << "First object: " << *this << ".";
		log::info::critical << "Second object: " << another << ".";
	}
	return guard;
}