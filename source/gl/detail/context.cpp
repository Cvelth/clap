#include "gl/detail/context.hpp"

#include "essential/log.hpp"

#include <unordered_map>
#include <mutex>

#include "GLFW/glfw3.h"

std::unordered_map<std::thread::id, clap::gl::detail::context *> current_context;
std::mutex map_access_mutex;

clap::gl::detail::context::activation_guard::activation_guard(context &context_ref)
	: essential::guard<std::mutex, detail::activate_context_callable, detail::deactivate_context_callable>(
		context_ref.activation_mutex,
		detail::activate_context_callable{ context_ref },
		detail::deactivate_context_callable{ context_ref }) {}

clap::gl::detail::context::access_guard::access_guard(context *context_ptr) : context_ptr(context_ptr) {
	if (context_ptr)
		context_ptr->access_mutex.lock_shared();
}
clap::gl::detail::context::access_guard::~access_guard() {
	if (context_ptr)
		context_ptr->access_mutex.unlock_shared();
}

clap::gl::detail::context::context(std::u8string name, size_t width, size_t height) :
	window(clap::gl::detail::window::create_windowed(name, width, height)) {}

clap::gl::detail::context::access_guard clap::gl::detail::context::current() {
	auto id = std::this_thread::get_id();

	std::lock_guard guard(map_access_mutex);
	auto current_iterator = current_context.find(id);
	return current_iterator != current_context.end() ? current_iterator->second : nullptr;
}

clap::gl::detail::context::activation_guard clap::gl::detail::context::make_current() {
	return activation_guard(*this);
}

void clap::gl::detail::context::activate() {
	auto id = std::this_thread::get_id();

	std::lock_guard map_guard(map_access_mutex);
	auto current_iterator = current_context.find(id);
	if (current_iterator != current_context.end() && current_iterator->second) {
		clap::log::error::critical << "An attempt to activate a context on a thread where a context is already activated.";
		clap::log::info::major << "An " << *current_iterator->second << " is currently active.";
		clap::log::info::major << "An " << *this << " is to be activated.";
	} else {
		std::lock_guard access_guard(access_mutex);

		current_context[id] = this;
		glfwMakeContextCurrent(*window);
		clap::log::message::minor << "Active context of thread #" << id << " was changed to " << *this << ".";
	}
}
void clap::gl::detail::context::deactivate() {
	auto id = std::this_thread::get_id();

	std::lock_guard guard(map_access_mutex);
	auto current_iterator = current_context.find(id);
	if (current_iterator != current_context.end() && current_iterator->second) {
		std::lock_guard access_guard(access_mutex);

		current_context[id] = nullptr;
		glfwMakeContextCurrent(nullptr);
		clap::log::message::minor << "Active context of thread #" << id << " was deactivated.";
		clap::log::info::major << *this << " was active.";
	} else
		clap::log::error::critical << "Destructor of context guard was called even though its constructor was never called. Fatal error.";
}