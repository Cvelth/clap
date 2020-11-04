#pragma once
#include <mutex>
#include <ostream>
#include <string>

#include "essential/guard.hpp"

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;
namespace clap::gl::detail {
	class context_guard;
}
namespace clap::gl::detail::window::event {
	class handler_interface;
}

namespace clap::gl::detail::window {
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

	class object;

	void initialize();
	void terminate();
	void ensure_initialized();

	object create(std::u8string title, size_t width, size_t height,
				  struct_handle<GLFWmonitor> monitor,
				  struct_handle<GLFWwindow> share = nullptr);
	object create_windowed(std::u8string title, size_t width, size_t height,
						   struct_handle<GLFWwindow> share = nullptr);
	object create_fullscreen(std::u8string title, size_t width, size_t height,
							 struct_handle<GLFWwindow> share = nullptr);
	object create_fullscreen(std::u8string title,
							 struct_handle<GLFWwindow> share = nullptr);
	object create_borderless(std::u8string title,
							 struct_handle<GLFWwindow> share = nullptr);

	void poll_events();
	void wait_events();
	void wait_events(double seconds);

	class object {
		friend object create(std::u8string title, size_t width, size_t height,
							 struct_handle<GLFWmonitor> monitor,
							 struct_handle<GLFWwindow> share);
		friend context_guard;

	public:
		~object();

		object(object const &another) = delete;
		object(object &&another) noexcept :
			handle(another.handle), event_handler(another.event_handler) {
			another.handle = nullptr;
			another.event_handler = nullptr;
		}
		object &operator=(object const &another) = delete;
		object &operator=(object &&another) noexcept {
			handle = another.handle;
			event_handler = another.event_handler;
			another.handle = nullptr;
			another.event_handler = nullptr;
		}

		operator struct_handle<GLFWwindow>() { return handle; }
		operator GLFWwindow *() { return *handle; }
		GLFWwindow *operator*() { return *handle; }
		GLFWwindow const *operator*() const { return *handle; }

		friend inline std::ostream &operator<<(std::ostream &stream, object const &another) {
			return stream << "window (glfw, titled \"" << (char const *) another.title.c_str() << "\")";
		}

		void swap_buffers();
		bool should_close();

		size_t width();
		size_t height();

		void set_event_handler(clap::gl::detail::window::event::handler_interface *event_handler);
		void remove_event_handler() { set_event_handler(nullptr); }

	protected:
		object(struct_handle<GLFWwindow> handle, std::u8string title) 
			: handle(handle), event_handler(nullptr), title(title) {}
	private:
		struct_handle<GLFWwindow> handle;
		clap::gl::detail::window::event::handler_interface *event_handler;
		std::u8string title;

		std::mutex mutex;
	};

	namespace event {
		enum class key {
			unknown, space, apostrophe, comma, minus, period, slash,
			n_1, n_2, n_3, n_4, n_5, n_6, n_7, n_8, n_9, n_0,
			semicolon, equal,
			a, b, c, d, e, f, g, h, i, j, k, l, m,
			n, o, p, q, r, s, t, u, v, w, x, y, z,
			bracket_left, backslash, bracket_right, grave_accent,
			world_1, world_2, escape, enter, tab, backspace,
			insert, delete_, right, left, down, up, page_up, page_down,
			home, end, caps_lock, scroll_lock, num_lock, print_screen,
			pause, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
			f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25,
			kp_1, kp_2, kp_3, kp_4, kp_5, kp_6, kp_7, kp_8, kp_9, kp_0,
			kp_point, kp_divide, kp_multiply,
			kp_subtract, kp_add, kp_enter, kp_equal,
			shift_left, control_left, alt_left, super_left,
			shift_right, control_right, alt_right, super_right,
			menu, last = menu,
		};
		enum class key_action {
			press, release, repeat
		};

		enum class mouse_button {
			b_1, b_2, b_3, b_4,
			b_5, b_6, b_7, b_8,

			last = b_8,
			left = b_1,
			right = b_2,
			middle = b_3
		};
		enum class mouse_button_action {
			press, release
		};

		enum class modificator_mask {
			none = 0x0, shift = 0x1,
			ctrl = 0x2, alt = 0x4,
			super = 0x8
		};
	}
}
clap::gl::detail::window::event::modificator_mask operator|(clap::gl::detail::window::event::modificator_mask lhs,
															clap::gl::detail::window::event::modificator_mask rhs);
clap::gl::detail::window::event::modificator_mask operator|=(clap::gl::detail::window::event::modificator_mask &lhs,
															 clap::gl::detail::window::event::modificator_mask rhs);
clap::gl::detail::window::event::modificator_mask operator&(clap::gl::detail::window::event::modificator_mask lhs,
															clap::gl::detail::window::event::modificator_mask rhs);
clap::gl::detail::window::event::modificator_mask operator&=(clap::gl::detail::window::event::modificator_mask &lhs,
															 clap::gl::detail::window::event::modificator_mask rhs);
clap::gl::detail::window::event::modificator_mask operator^(clap::gl::detail::window::event::modificator_mask lhs,
															clap::gl::detail::window::event::modificator_mask rhs);
clap::gl::detail::window::event::modificator_mask operator^=(clap::gl::detail::window::event::modificator_mask &lhs,
															 clap::gl::detail::window::event::modificator_mask rhs);

#pragma warning(push)
#pragma warning(disable : 4100)

namespace clap::gl::detail::window::event {
	namespace detail {
		class impl;
	}

	class handler_interface {
		friend clap::gl::detail::window::event::detail::impl;
	protected:
		inline virtual bool on_key(event::key key, int scancode, event::key_action action,
								   event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_character(char32_t codepoint) {
			return false;
		}
		inline virtual bool on_character(char32_t codepoint, event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_mouse_button(event::mouse_button button, event::mouse_button_action action,
											event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_mouse_move(double x, double y) {
			return false;
		}
		inline virtual bool on_mouse_entering() {
			return false;
		}
		inline virtual bool on_mouse_leaving() {
			return false;
		}
		inline virtual bool on_scroll(double dx, double dy) {
			return false;
		}
		inline virtual bool on_window_resize(size_t x, size_t y) {
			return false;
		}
		inline virtual bool on_file_drop(int count, const char8_t **paths) {
			return false;
		}

		inline virtual bool on_window_close() {
			return false;
		}
		inline virtual bool on_window_move(size_t x, size_t y) {
			return false;
		}
		inline virtual bool on_window_focus_gain() {
			return false;
		}
		inline virtual bool on_window_focus_loss() {
			return false;
		}
		inline virtual bool on_window_minimize() {
			return false;
		}
		inline virtual bool on_restore_minimized() {
			return false;
		}
		inline virtual bool on_window_maximize() {
			return false;
		}
		inline virtual bool on_restore_maximized() {
			return false;
		}
	};
}

#pragma warning(pop)

namespace clap::gl::detail::convert {
	int to_glfw(window::event::key v);
	window::event::key to_key(int v);
	int to_glfw(window::event::key_action v);
	window::event::key_action to_key_action(int v);

	int to_glfw(window::event::mouse_button  v);
	window::event::mouse_button to_mouse_button(int v);
	int to_glfw(window::event::mouse_button_action v);
	window::event::mouse_button_action to_mouse_button_action(int v);

	int to_glfw(window::event::modificator_mask v);
	window::event::modificator_mask to_modificator_mask(int v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::key v);
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::key_action v);
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::mouse_button v);
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::mouse_button_action v);
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::modificator_mask v);