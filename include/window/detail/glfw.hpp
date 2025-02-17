﻿#pragma once
#include <string>

struct GLFWwindow;
struct GLFWmonitor;
struct GLFWvidmode;

namespace clap::gl::detail {
	class state;
}
namespace clap::event {
	class handler_interface;
}

namespace clap::detail {
	namespace detail {
		template <typename glfw_struct>
		class glfw_struct_handle {
		public:
			glfw_struct_handle(glfw_struct *pointer) : pointer(pointer) {}

			operator glfw_struct *() { return pointer; }
			glfw_struct *operator*() { return pointer; }
			glfw_struct const *operator*() const { return pointer; }
			glfw_struct *operator->() { return pointer; }
			glfw_struct const *operator->() const { return pointer; }
		private:
			glfw_struct *pointer;
		};
		using glfw_window_handle = glfw_struct_handle<GLFWwindow>;
		using glfw_monitor_handle = glfw_struct_handle<GLFWmonitor>;
		using glfw_video_mode_handle = glfw_struct_handle<GLFWvidmode>;
		using glfw_const_video_mode_handle = glfw_struct_handle<GLFWvidmode const>;
	}

	class glfw_window {
	public:
		glfw_window(detail::glfw_window_handle handle) : handle(handle) {}
		~glfw_window() {
			remove_event_handler();
		}

		operator detail::glfw_window_handle() { return handle; }
		detail::glfw_window_handle operator*() { return handle; }
		detail::glfw_window_handle const operator*() const { return handle; }
		detail::glfw_window_handle operator->() { return handle; }
		detail::glfw_window_handle const operator->() const { return handle; }

		void destroy();
		void update();
		bool should_close();
		void make_current();

		size_t width();
		size_t height();

		void set_event_handler(clap::event::handler_interface *handler_interface);
		void remove_event_handler() { set_event_handler(nullptr); }

	private:
		detail::glfw_window_handle handle;
	};

	class glfw_video_mode {
	public:
		glfw_video_mode(detail::glfw_const_video_mode_handle handle) : handle(handle) {}

		operator detail::glfw_const_video_mode_handle() { return handle; }
		detail::glfw_const_video_mode_handle operator*() { return handle; }
		detail::glfw_const_video_mode_handle const operator*() const { return handle; }
		detail::glfw_const_video_mode_handle operator->() { return handle; }
		detail::glfw_const_video_mode_handle const operator->() const { return handle; }

		size_t width() const;
		size_t height() const;
		int red_bits() const;
		int green_bits() const;
		int blue_bits() const;
		int refresh_rate() const;

	private:
		const detail::glfw_const_video_mode_handle handle;
	};

	class glfw {
		friend glfw_window;
		friend gl::detail::state;
	public:
		static void initialize();
		static void terminate();

		static glfw_window create_window(size_t width, size_t height, std::string title,
										 detail::glfw_monitor_handle monitor, 
										 detail::glfw_window_handle share = nullptr);
		static glfw_window create_window_windowed(size_t width, size_t height, std::string title,
												  detail::glfw_window_handle share = nullptr);
		static glfw_window create_window_fullscreen(size_t width, size_t height, std::string title,
													detail::glfw_window_handle share = nullptr);
		static glfw_window create_window_fullscreen(std::string title,
													detail::glfw_window_handle share = nullptr);
		static glfw_window create_window_borderless(std::string title,
													detail::glfw_window_handle share = nullptr);

		static void poll_events();
		static void wait_events();
		static void wait_events(double seconds);

		static detail::glfw_monitor_handle primary_monitor();
		static glfw_video_mode const video_mode(detail::glfw_monitor_handle monitor);

		static glfw_video_mode const primary_monitor_video_mode() {
			return video_mode(primary_monitor());
		}

		glfw() = delete;
		glfw(glfw const &other) = delete;
		glfw(glfw &&other) = delete;

	protected:
		static void initialize_implicitly();
		static void terminate_implicitly();

		inline static void ensure_initialization() {
			if (!is_initialized)
				initialize_implicitly();
		}
	private:
		static bool is_initialized;
		static bool is_context_selected;
	};
}

#include "window/event_type.hpp"
namespace clap::event::detail::convert {
	int to_glfw(key v);
	key to_key(int v);
	int to_glfw(key_action v);
	key_action to_key_action(int v);

	int to_glfw(mouse_button v);
	mouse_button to_mouse_button(int v);	
	int to_glfw(mouse_button_action v);
	mouse_button_action to_mouse_button_action(int v);
	int to_glfw(mouse_hover_action v);
	mouse_hover_action to_mouse_hover_action(int v);

	int to_glfw(modificator_mask v);
	modificator_mask to_modificator_mask(int v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &s, clap::event::key v);
std::ostream &operator<<(std::ostream &s, clap::event::key_action v);
std::ostream &operator<<(std::ostream &s, clap::event::mouse_button v);
std::ostream &operator<<(std::ostream &s, clap::event::mouse_button_action v);
std::ostream &operator<<(std::ostream &s, clap::event::mouse_hover_action v);
std::ostream &operator<<(std::ostream &s, clap::event::modificator_mask v);