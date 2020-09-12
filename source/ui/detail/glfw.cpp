﻿#include "ui/detail/glfw.hpp"
#include "ui/detail/event.hpp"

#include <unordered_map>

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "gl/detail/state.hpp"

#include "essential/guard.hpp"
#include "essential/log.hpp"

bool is_initialized = false;
//bool is_context_selected = false;

void clap::ui::detail::glfw::initialize() {
	if (!is_initialized) {
		if (!glfwInit())
			log::warning::critical << "GLFW initialization error.";
		else {
			log::message::major << "GLFW was initialized.";
			log::info::critical << "Version: " << glfwGetVersionString();

			is_initialized = true;
		}
	}
}

void clap::ui::detail::glfw::terminate() {
	if (is_initialized) {
		glfwTerminate();
		log::message::major << "GLFW was terminated.";
		is_initialized = false;
	}
}

void clap::ui::detail::glfw::ensure_initialized() {
	initialize();
}

void clap::gl::detail::load_gl() {
	if (!clap::gl::detail::state::was_loaded) {
		bool success = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		if (success) {
			clap::gl::detail::state::was_loaded = true;

			clap::log::message::critical << "GL was loaded using 'glad'.";
			clap::log::info::critical << "GL Version: " << glGetString(GL_VERSION);
			clap::log::info::negligible << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
			clap::log::info::negligible << "Vendor: " << glGetString(GL_VENDOR);
			clap::log::info::major << "Renderer: " << glGetString(GL_RENDERER);
		} else
			clap::log::warning::critical << "Unable to load GL. 'glad' returns false.";
	}
}

clap::ui::detail::glfw::window_handle clap::ui::detail::glfw::create_window(std::u8string title,
																			size_t width, size_t height,
																			struct_handle<GLFWmonitor> monitor,
																			struct_handle<GLFWwindow> share) {
	ensure_initialized();

	int signed_width = int(width);
	int signed_height = int(height);
	if (width != size_t(signed_width) || height != size_t(signed_height)) {
		log::warning::critical << "Window dimention value overflow. They are too large.";
		log::info::major << "Dimentions are: (" << width << ", " << height << ").";
	}

	if (auto out = glfwCreateWindow(signed_width, signed_height, (char const *) title.c_str(), monitor, share); out) {
		log::message::major << "New window was created with dimentions (" << width << ", " << height << ").";

		essential::guard guard(out,
							   [](auto handle) { glfwMakeContextCurrent(handle); },
							   [](auto handle) { /*glfwMakeContextCurrent(nullptr);*/ });
		clap::gl::detail::load_gl();

		return window_handle(out);
	} else {
		log::error::major << "Unable to initialize 'GLFWwindow'.";
		log::info::major << "'glfwCreateWindow' has returned a 'nullptr'.";
	}
}
clap::ui::detail::glfw::window_handle clap::ui::detail::glfw::create_window_windowed(std::u8string title,
																					 size_t width, size_t height,
																					 struct_handle<GLFWwindow> share) {
	return create_window(title, width, height, nullptr, share);
}
clap::ui::detail::glfw::window_handle clap::ui::detail::glfw::create_window_fullscreen(std::u8string title,
																					   size_t width, size_t height,
																					   struct_handle<GLFWwindow> share) {
	ensure_initialized();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor)
		log::warning::minor << "Unable to access monitor data. "
		"Creating window in 'windowed' mode instead of 'fullscreen'.";
	return create_window(title, width, height, primary_monitor, share);
}
clap::ui::detail::glfw::window_handle clap::ui::detail::glfw::create_window_fullscreen(std::u8string title,
																					   struct_handle<GLFWwindow> share) {
	ensure_initialized();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor)
		log::warning::minor << "Unable to access monitor data. "
		"Creating window in 'windowed' mode instead of 'fullscreen'.";
	auto video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {
		log::warning::minor << "Unable to obtain video data from a monitor. "
			"Window resolution is set to (1280, 720) instead of monitor size.";
		return create_window(title, 1280, 720, primary_monitor, share);
	} else
		return create_window(title, size_t(video_mode->width), size_t(video_mode->height),
							 primary_monitor, share);
}
clap::ui::detail::glfw::window_handle clap::ui::detail::glfw::create_window_borderless(std::u8string title,
																					   struct_handle<GLFWwindow> share) {
	ensure_initialized();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor)
		log::warning::minor << "Unable to access monitor data. "
		"Creating window in 'windowed' mode instead of 'borderless'.";
	auto video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {
		log::warning::minor << "Unable to obtain video data from a monitor. "
			"Creating window in 'windowed' mode instead of 'borderless'. "
			"Window resolution is set to (1280, 720) instead of monitor size.";;
		return create_window(title, 1280, 720, primary_monitor, share);
	} else {
		glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
		return create_window(title, size_t(video_mode->width), size_t(video_mode->height),
							 primary_monitor, share);
	}
}

static void clap::ui::detail::glfw::poll_events() {
	ensure_initialized();
	glfwPollEvents();
}
static void clap::ui::detail::glfw::wait_events() {
	ensure_initialized();
	glfwWaitEvents();
}
static void clap::ui::detail::glfw::wait_events(double seconds) {
	ensure_initialized();
	glfwWaitEventsTimeout(seconds);
}
clap::ui::detail::glfw::window_handle::~window_handle() {
	if (event_handler)
		remove_event_handler();
	if (handle) {
		glfwDestroyWindow(handle);
		log::message::major << "A window was destroyed.";
	}
}
void clap::ui::detail::glfw::window_handle::swap_buffers() {
	glfwSwapBuffers(handle);
	//log::message::negligible << "A window was updated.";
}
bool clap::ui::detail::glfw::window_handle::should_close() {
	return glfwWindowShouldClose(handle);
}

size_t clap::ui::detail::glfw::window_handle::width() {
	int out;
	glfwGetFramebufferSize(handle, &out, nullptr);
	return size_t(out);
}
size_t clap::ui::detail::glfw::window_handle::height() {
	int out;
	glfwGetFramebufferSize(handle, nullptr, &out);
	return size_t(out);
}

namespace clap::ui::detail::event::detail {
	class impl {
	private:
		static std::unordered_map<GLFWwindow *, event::handler_interface *> mapping;
	protected:
		inline static clap::ui::detail::event::handler_interface *get(GLFWwindow *key) {
			auto ptr = mapping.find(key);
			return ptr != mapping.end() ? ptr->second : nullptr;
		}
	public:
		static void push(GLFWwindow *window_handle, clap::ui::detail::event::handler_interface *event_handler) {
			if (!window_handle)
				clap::log::warning::critical << "Cannot use 'nullptr' window for event handling.";
			else if (!event_handler)
				clap::log::warning::critical << "Cannot use 'nullptr' event_handler.";
			else
				mapping.insert(std::pair(window_handle, event_handler));
		}
		static void pop(GLFWwindow *window_handle) {
			if (!window_handle)
				clap::log::warning::critical << "Cannot use 'nullptr' window to disable events.";
			else {
				auto iterator = mapping.find(window_handle);
				if (iterator != mapping.end())
					mapping.erase(iterator);
			}
		}
	public:
		static void on_key(GLFWwindow *w, int key_raw, int scancode_raw, int action_raw, int modificators_raw) {
			auto key = clap::ui::detail::event::convert::to_key(key_raw);
			auto action = clap::ui::detail::event::convert::to_key_action(action_raw);
			auto modificators = clap::ui::detail::event::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_key(key, scancode_raw, action, modificators))
					clap::log::warning::minor << "Key event \'" << modificators << key
					<< " was " << action << "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Key event \'" << modificators << key <<
				" was " << action << "\' has no handler installed.";
		}
		static void on_character(GLFWwindow *w, unsigned int codepoint_raw) {
			auto codepoint = static_cast<char32_t>(codepoint_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw))
					clap::log::warning::minor << "Character event \'" << codepoint
					<< "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Character event \'" << codepoint
				<< "\' has no handler installed.";
		}
		static void on_character(GLFWwindow *w, unsigned int codepoint_raw, int modificators_raw) {
			auto codepoint = static_cast<char32_t>(codepoint_raw);
			auto modificators = clap::ui::detail::event::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw))
					clap::log::warning::minor << "Character event \'" << modificators << codepoint
					<< "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Character event \'" << modificators << codepoint
				<< "\' has no handler installed.";
		}
		static void on_mouse_button(GLFWwindow *w, int button_raw, int action_raw, int modificators_raw) {
			auto button = clap::ui::detail::event::convert::to_mouse_button(button_raw);
			auto action = clap::ui::detail::event::convert::to_mouse_button_action(action_raw);
			auto modificators = clap::ui::detail::event::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_mouse_button(button, action, modificators))
					clap::log::warning::minor << "Mouse event \'" << modificators << button
					<< " was " << action << "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Mouse event \'" << modificators << button <<
				" was " << action << "\' has no handler installed.";
		}
		static void on_mouse_move(GLFWwindow *w, double x, double y) {
			if (auto handler = get(w); handler) {
				if (!handler->on_mouse_move(x, y))
					clap::log::warning::minor << "Mouse event \'Move to (" << x << ", " << y
					<< ")\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Mouse event \'Move to (" << x << ", " << y
				<< ")\' has no handler installed.";
		}
		static void on_mouse_hover(GLFWwindow *w, int enter_flag_raw) {
			auto action = clap::ui::detail::event::convert::to_mouse_hover_action(enter_flag_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_mouse_hover(action))
					clap::log::warning::minor << "Mouse event \'Hovering "
					<< (enter_flag_raw ? "in" : "out") << "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Mouse event \'Hovering "
				<< (enter_flag_raw ? "in" : "out") << "\' has no handler installed.";
		}
		static void on_scroll(GLFWwindow *w, double dx, double dy) {
			if (auto handler = get(w); handler) {
				if (!handler->on_scroll(dx, dy))
					clap::log::warning::minor << "Scroll event \'Delta is (" << dx << ", " << dy
					<< ")\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Scroll event \'Delta is (" << dx << ", " << dy
				<< "\' has no handler installed.";
		}
		static void on_resize(GLFWwindow *w, int x_raw, int y_raw) {
			size_t x = x_raw, y = y_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_resize(x, y))
					clap::log::warning::minor << "Resize event \'New size is (" << x << ", " << y
					<< ")\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Resize event \'New size is (" << x << ", " << y
				<< "\' has no handler installed.";
		}
		static void on_file_drop(GLFWwindow *w, int count, char const **paths_raw) {
			auto paths = (char8_t const **) paths_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_file_drop(count, paths)) {
					clap::log::warning::minor << "File drop event wasn't handled (handler returned 'false').";
					clap::log::info::major << "Count: " << count;
					for (int i = 0; i < count; i++)
						clap::log::info::minor << i << ": '" << paths[i] << "'.";
				}
			} else {
				clap::log::warning::minor << "File drop event has no handler installed.";
				clap::log::info::major << "Count: " << count;
				for (int i = 0; i < count; i++)
					clap::log::info::minor << i << ": '" << paths[i] << "'.";
			}
		}
	};
}
std::unordered_map<GLFWwindow *, clap::ui::detail::event::handler_interface *> clap::ui::detail::event::detail::impl::mapping = {};

void clap::ui::detail::glfw::window_handle::set_event_handler(clap::ui::detail::event::handler_interface *handler_interface) {
	if (handler_interface) {
		clap::ui::detail::event::detail::impl::push(*handle, handler_interface);
		event_handler = handler_interface;

		glfwSetKeyCallback(*handle, clap::ui::detail::event::detail::impl::on_key);
		glfwSetMouseButtonCallback(*handle, clap::ui::detail::event::detail::impl::on_mouse_button);
		glfwSetCharCallback(*handle, clap::ui::detail::event::detail::impl::on_character);
		glfwSetCharModsCallback(*handle, clap::ui::detail::event::detail::impl::on_character);
		glfwSetCursorEnterCallback(*handle, clap::ui::detail::event::detail::impl::on_mouse_hover);
		glfwSetCursorPosCallback(*handle, clap::ui::detail::event::detail::impl::on_mouse_move);
		glfwSetDropCallback(*handle, clap::ui::detail::event::detail::impl::on_file_drop);
		glfwSetScrollCallback(*handle, clap::ui::detail::event::detail::impl::on_scroll);
		glfwSetFramebufferSizeCallback(*handle, clap::ui::detail::event::detail::impl::on_resize);

	} else
		if (event_handler) {
			clap::ui::detail::event::detail::impl::pop(*handle);
			event_handler = nullptr;
		}
}