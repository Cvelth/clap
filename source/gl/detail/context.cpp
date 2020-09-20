#include "gl/detail/context.hpp"

#include "essential/log.hpp"

#include <unordered_map>
#include <mutex>

#include "GLFW/glfw3.h"

std::mutex mutex;
std::unordered_map<std::thread::id, clap::gl::detail::context *> current_context;

clap::gl::detail::context_guard::context_guard(context &context_ref) :
	essential::guard<std::mutex, detail::lock_context_callable, detail::unlock_context_callable>(
		context_ref.mutex,
		detail::lock_context_callable{ context_ref },
		detail::unlock_context_callable{ context_ref }) {}
void clap::gl::detail::detail::lock_context_callable::operator()() {
	auto id = std::this_thread::get_id();

	std::lock_guard guard(mutex);
	auto current_iterator = current_context.find(id);
	if (current_iterator != current_context.end() && current_iterator->second)
		if (current_iterator->second == &context_ref) {
			current_iterator->second->guard_counter++;
			return;
		} else
			if (context_ref.previous)
				clap::log::error::critical << "Cannot make a context current. Its 'previous' pointer is already set.";
			else
				context_ref.previous = current_iterator->second;

		current_context[id] = &context_ref;
		context_ref.guard_counter++;
		context_ref.activate();
}
void clap::gl::detail::detail::unlock_context_callable::operator()() {
	auto id = std::this_thread::get_id();

	std::lock_guard guard(mutex);
	auto current_iterator = current_context.find(id);
	if (current_iterator != current_context.end() && current_iterator->second) {
		if (current_iterator->second == &context_ref)
			if (--context_ref.guard_counter) {
				context *working = &context_ref;
				while (working->previous) {
					if (working->previous->guard_counter) {
						current_iterator->second = working->previous;
						return;
					} else
						working = working->previous;
				}
				working->deactivate();
			} else
				return;
		else
			context_ref.guard_counter--;
	} else
		clap::log::error::critical << "Destructor of context guard was called even though its constructor was never called. Fatal error.";
}

clap::gl::detail::context::context(std::u8string name, size_t width, size_t height) : 
	window(clap::gl::detail::window::create_windowed(name, width, height)), 
	previous(nullptr), guard_counter(0u) {}

clap::gl::detail::context* clap::gl::detail::context::current() {
	auto id = std::this_thread::get_id();

	std::lock_guard guard(::mutex);
	auto current_iterator = current_context.find(id);
	return current_iterator != current_context.end() ? current_iterator->second : nullptr;
}

clap::gl::detail::context_guard clap::gl::detail::context::make_current() {
	return context_guard(*this);
}

void clap::gl::detail::context::activate() { glfwMakeContextCurrent(*window); }
void clap::gl::detail::context::deactivate() { glfwMakeContextCurrent(nullptr); }