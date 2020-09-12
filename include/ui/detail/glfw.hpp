#pragma once
#include <mutex>
#include <string>

#include "essential/guard.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;
namespace clap::gl::detail {
	class state;
}
namespace clap::ui::detail::event {
	class handler_interface;
}

namespace clap::ui::detail::glfw {
	template <typename glfw_type>
	class struct_handle {
	public:
		struct_handle(glfw_type *pointer) : pointer(pointer) {}

		operator glfw_type *() { return pointer; }
		glfw_type *operator*() { return pointer; }
		glfw_type const *operator*() const { return pointer; }
		glfw_type *operator->() { return pointer; }
		glfw_type const *operator->() const { return pointer; }
	private:
		glfw_type *pointer;
	};

	class window_handle;
	class context_guard;

	void initialize();
	void terminate();
	void ensure_initialized();

	window_handle create_window(std::u8string title, size_t width, size_t height,
								struct_handle<GLFWmonitor> monitor,
								struct_handle<GLFWwindow> share = nullptr);
	window_handle create_window_windowed(std::u8string title, size_t width, size_t height,
										 struct_handle<GLFWwindow> share = nullptr);
	window_handle create_window_fullscreen(std::u8string title, size_t width, size_t height,
										   struct_handle<GLFWwindow> share = nullptr);
	window_handle create_window_fullscreen(std::u8string title,
										   struct_handle<GLFWwindow> share = nullptr);
	window_handle create_window_borderless(std::u8string title,
										   struct_handle<GLFWwindow> share = nullptr);

	void poll_events();
	void wait_events();
	void wait_events(double seconds);

	class window_handle {
		friend window_handle create_window(std::u8string title, size_t width, size_t height,
										   struct_handle<GLFWmonitor> monitor,
										   struct_handle<GLFWwindow> share);
		friend class context_guard;

	public:
		~window_handle();

		window_handle(window_handle const &another) = delete;
		window_handle(window_handle &&another) noexcept :
			handle(another.handle), event_handler(another.event_handler) {
			another.handle = nullptr;
			another.event_handler = nullptr;
		}
		window_handle &operator=(window_handle const &another) = delete;
		window_handle &operator=(window_handle &&another) noexcept {
			handle = another.handle;
			event_handler = another.event_handler;
			another.handle = nullptr;
			another.event_handler = nullptr;
		}

		operator struct_handle<GLFWwindow>() { return handle; }
		operator GLFWwindow *() { return *handle; }
		GLFWwindow *operator*() { return *handle; }
		GLFWwindow const *operator*() const { return *handle; }

		void swap_buffers();
		bool should_close();

		size_t width();
		size_t height();

		void set_event_handler(clap::ui::detail::event::handler_interface *event_handler);
		void remove_event_handler() { set_event_handler(nullptr); }

	protected:
		window_handle(struct_handle<GLFWwindow> handle) : handle(handle), event_handler(nullptr) {}
	private:
		struct_handle<GLFWwindow> handle;
		clap::ui::detail::event::handler_interface *event_handler;

		std::mutex mutex;
	};

	namespace detail {
		struct lock_context_callable {
			window_handle &context_owner;
			void operator()();
		};
		struct unlock_context_callable {
			window_handle &context_owner;
			void operator()();
		};
	}
	class context_guard
		: public essential::guard<std::mutex, detail::lock_context_callable, detail::unlock_context_callable> {
	public:
		context_guard(window_handle &context_owner) :
			essential::guard<std::mutex, detail::lock_context_callable, detail::unlock_context_callable>(
				context_owner.mutex,
				detail::lock_context_callable{ context_owner },
				detail::unlock_context_callable{ context_owner }) {}
	};
}