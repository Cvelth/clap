#include "gl/detail/window.hpp"
#include "gl/detail/state.hpp"

#include <unordered_map>

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "essential/guard.hpp"
#include "essential/log.hpp"

bool is_initialized = false;
//bool is_context_selected = false;

void clap::gl::detail::window::initialize() {
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

void clap::gl::detail::window::terminate() {
	if (is_initialized) {
		glfwTerminate();
		log::message::major << "GLFW was terminated.";
		is_initialized = false;
	}
}

void clap::gl::detail::window::ensure_initialized() {
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

clap::gl::detail::window::object clap::gl::detail::window::create(std::u8string title,
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

		glfwMakeContextCurrent(out);
		clap::gl::detail::load_gl();
		glfwMakeContextCurrent(nullptr);

		return object(out);
	} else {
		log::error::major << "Unable to initialize 'GLFWwindow'.";
		log::info::major << "'glfwCreateWindow' has returned a 'nullptr'.";
	}
}
clap::gl::detail::window::object clap::gl::detail::window::create_windowed(std::u8string title,
																		   size_t width, size_t height,
																		   struct_handle<GLFWwindow> share) {
	return create(title, width, height, nullptr, share);
}
clap::gl::detail::window::object clap::gl::detail::window::create_fullscreen(std::u8string title,
																			 size_t width, size_t height,
																			 struct_handle<GLFWwindow> share) {
	ensure_initialized();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor)
		log::warning::minor << "Unable to access monitor data. "
		"Creating window in 'windowed' mode instead of 'fullscreen'.";
	return create(title, width, height, primary_monitor, share);
}
clap::gl::detail::window::object clap::gl::detail::window::create_fullscreen(std::u8string title,
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
		return create(title, 1280, 720, primary_monitor, share);
	} else
		return create(title, size_t(video_mode->width), size_t(video_mode->height),
							 primary_monitor, share);
}
clap::gl::detail::window::object clap::gl::detail::window::create_borderless(std::u8string title,
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
		return create(title, 1280, 720, primary_monitor, share);
	} else {
		glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
		return create(title, size_t(video_mode->width), size_t(video_mode->height),
							 primary_monitor, share);
	}
}

void clap::gl::detail::window::poll_events() {
	ensure_initialized();
	glfwPollEvents();
}
void clap::gl::detail::window::wait_events() {
	ensure_initialized();
	glfwWaitEvents();
}
void clap::gl::detail::window::wait_events(double seconds) {
	ensure_initialized();
	glfwWaitEventsTimeout(seconds);
}
clap::gl::detail::window::object::~object() {
	if (event_handler)
		remove_event_handler();
	if (handle) {
		glfwDestroyWindow(handle);
		log::message::major << "A window was destroyed.";
	}
}
void clap::gl::detail::window::object::swap_buffers() {
	glfwSwapBuffers(handle);
	//log::message::negligible << "A window was updated.";
}
bool clap::gl::detail::window::object::should_close() {
	return glfwWindowShouldClose(handle);
}

size_t clap::gl::detail::window::object::width() {
	int out;
	glfwGetFramebufferSize(handle, &out, nullptr);
	return size_t(out);
}
size_t clap::gl::detail::window::object::height() {
	int out;
	glfwGetFramebufferSize(handle, nullptr, &out);
	return size_t(out);
}

namespace clap::gl::detail::window::event::detail {
	class impl {
	private:
		static std::unordered_map<GLFWwindow *, event::handler_interface *> mapping;
	protected:
		inline static event::handler_interface *get(GLFWwindow *key) {
			auto ptr = mapping.find(key);
			return ptr != mapping.end() ? ptr->second : nullptr;
		}
	public:
		static void push(GLFWwindow *object, event::handler_interface *event_handler) {
			if (!object)
				clap::log::warning::critical << "Cannot use 'nullptr' window for event handling.";
			else if (!event_handler)
				clap::log::warning::critical << "Cannot use 'nullptr' event_handler.";
			else
				mapping.insert(std::pair(object, event_handler));
		}
		static void pop(GLFWwindow *object) {
			if (!object)
				clap::log::warning::critical << "Cannot use 'nullptr' window to disable events.";
			else {
				auto iterator = mapping.find(object);
				if (iterator != mapping.end())
					mapping.erase(iterator);
			}
		}
	public:
		static void on_key(GLFWwindow *w, int key_raw, int scancode_raw, int action_raw, int modificators_raw) {
			auto key = clap::gl::detail::convert::to_key(key_raw);
			auto action = clap::gl::detail::convert::to_key_action(action_raw);
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
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
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw))
					clap::log::warning::minor << "Character event \'" << modificators << codepoint
					<< "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Character event \'" << modificators << codepoint
				<< "\' has no handler installed.";
		}
		static void on_mouse_button(GLFWwindow *w, int button_raw, int action_raw, int modificators_raw) {
			auto button = clap::gl::detail::convert::to_mouse_button(button_raw);
			auto action = clap::gl::detail::convert::to_mouse_button_action(action_raw);
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
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
			if (auto handler = get(w); handler) {
				if (enter_flag_raw) {
					if (!handler->on_mouse_entering())
						clap::log::warning::minor << "Mouse event \'Entering\' wasn't handled (handler returned 'false').";
				} else
					if (!handler->on_mouse_leaving())
						clap::log::warning::minor << "Mouse event \'Leaving\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Mouse events \'Entering\' and \'Leaving\' have no handler installed.";
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
		static void on_window_resize(GLFWwindow *w, int x_raw, int y_raw) {
			size_t x = x_raw, y = y_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_window_resize(x, y))
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

		static void on_window_move(GLFWwindow *w, int x_raw, int y_raw) {
			size_t x = x_raw, y = y_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_window_move(x, y))
					clap::log::warning::minor << "Move event \'New position is (" << x << ", " << y
					<< ")\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Move event \'New position is (" << x << ", " << y
				<< "\' has no handler installed.";
		}
		static void on_window_close(GLFWwindow *w) {
			if (auto handler = get(w); handler) {
				if (!handler->on_window_close())
					clap::log::warning::minor << "Close event wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Close event has no handler installed.";
		}
		static void on_window_focus(GLFWwindow *w, int focus_flag_raw) {
			if (auto handler = get(w); handler) {
				if (focus_flag_raw) {
					if (!handler->on_window_focus_gain())
						clap::log::warning::minor << "Focus gaining event wasn't handled (handler returned 'false').";
				} else
					if (!handler->on_window_focus_loss())
						clap::log::warning::minor << "Focus lossing event wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Focus events have no handler installed.";
		}
		static void on_window_minimize(GLFWwindow *w, int flag_raw) {
			if (auto handler = get(w); handler) {
				if (flag_raw) {
					if (!handler->on_window_minimize())
						clap::log::warning::minor << "Minimization event wasn't handled (handler returned 'false').";
				} else
					if (!handler->on_restore_minimized())
						clap::log::warning::minor << "Minimization event wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Minimization events have no handler installed.";
		}
		static void on_window_maximize(GLFWwindow *w, int flag_raw) {
			if (auto handler = get(w); handler) {
				if (flag_raw) {
					if (!handler->on_window_maximize())
						clap::log::warning::minor << "Minimization event wasn't handled (handler returned 'false').";
				} else
					if (!handler->on_restore_maximized())
						clap::log::warning::minor << "Minimization event wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Minimization events have no handler installed.";
		}
	};
}
std::unordered_map<GLFWwindow *, clap::gl::detail::window::event::handler_interface *> clap::gl::detail::window::event::detail::impl::mapping = {};

void clap::gl::detail::window::object::set_event_handler(clap::gl::detail::window::event::handler_interface *handler_interface) {
	if (handler_interface) {
		clap::gl::detail::window::event::detail::impl::push(*handle, handler_interface);
		event_handler = handler_interface;

		glfwSetKeyCallback(*handle, clap::gl::detail::window::event::detail::impl::on_key);
		glfwSetMouseButtonCallback(*handle, clap::gl::detail::window::event::detail::impl::on_mouse_button);
		glfwSetCharCallback(*handle, clap::gl::detail::window::event::detail::impl::on_character);
		glfwSetCharModsCallback(*handle, clap::gl::detail::window::event::detail::impl::on_character);
		glfwSetCursorEnterCallback(*handle, clap::gl::detail::window::event::detail::impl::on_mouse_hover);
		glfwSetCursorPosCallback(*handle, clap::gl::detail::window::event::detail::impl::on_mouse_move);
		glfwSetDropCallback(*handle, clap::gl::detail::window::event::detail::impl::on_file_drop);
		glfwSetScrollCallback(*handle, clap::gl::detail::window::event::detail::impl::on_scroll);
		glfwSetFramebufferSizeCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_resize);
		glfwSetWindowPosCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_move);
		glfwSetWindowCloseCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_close);
		glfwSetWindowFocusCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_focus);
		glfwSetWindowIconifyCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_minimize);
		glfwSetWindowMaximizeCallback(*handle, clap::gl::detail::window::event::detail::impl::on_window_maximize);

	} else
		if (event_handler) {
			clap::gl::detail::window::event::detail::impl::pop(*handle);
			event_handler = nullptr;
		}
}

void clap::gl::detail::window::detail::lock_context_callable::operator()() {
	glfwMakeContextCurrent(context_owner);
}
void clap::gl::detail::window::detail::unlock_context_callable::operator()() {
	glfwMakeContextCurrent(nullptr);
}
