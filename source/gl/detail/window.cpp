#include "gl/detail/window.hpp"

#include <unordered_map>

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "essential/guard.hpp"
#include "essential/log.hpp"

static bool is_initialized = false;
void clap::gl::detail::window::initialize() {
	if (!is_initialized) {
		if (!glfwInit())
			log::warning::critical << "Fail to initialize GLFW.";
		else {
			log::message::major << "Initialize GLFW.";
			log::info::critical << "Version: " << glfwGetVersionString();

			is_initialized = true;
		}
	}
}
void clap::gl::detail::window::terminate() {
	if (is_initialized) {
		glfwTerminate();
		log::message::major << "Terminate GLFW.";
		is_initialized = false;
	}
}
void initialize_event_handling(GLFWwindow *);

clap::gl::detail::window::object clap::gl::detail::window::create(std::u8string title,
																  size_t width, size_t height,
																  struct_handle<GLFWmonitor> monitor,
																  struct_handle<GLFWwindow> share) {
	initialize();

	int signed_width = int(width);
	int signed_height = int(height);
	if (width != size_t(signed_width) || height != size_t(signed_height)) {
		log::warning::critical << "Encounter a window dimention value overflow.";
		log::info::major << "Dimentions are: (" << width << ", " << height << ").";
	}

	if (auto out = glfwCreateWindow(signed_width, signed_height, (char const *) title.c_str(), monitor, share); out) {
		log::message::major << "Create a new window with dimentions (" << width << ", " << height << ") titled \"" << title << "\".";

		static bool was_gl_loaded = false;
		if (!was_gl_loaded) {
			glfwMakeContextCurrent(out);

			bool success = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			if (success) {
				was_gl_loaded = true;

				clap::log::message::critical << "Load GL using 'glad'.";
				clap::log::info::critical << "GL Version: " << glGetString(GL_VERSION);
				clap::log::info::negligible << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
				clap::log::info::negligible << "Vendor: " << glGetString(GL_VENDOR);
				clap::log::info::major << "Renderer: " << glGetString(GL_RENDERER);
			} else {
				clap::log::warning::critical << "Fail to load GL.";
				clap::log::info::major << "'gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)' returned false.";
			}

			glfwMakeContextCurrent(nullptr);
		}

		initialize_event_handling(out);
		return object(out, title);
	} else {
		log::error::major << "Fail to create a window.";
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
	initialize();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {
		log::warning::minor << "Fail to access monitor data.\n"
			"Created window is in 'windowed' mode instead of 'fullscreen'.";
	}
	return create(title, width, height, primary_monitor, share);
}
clap::gl::detail::window::object clap::gl::detail::window::create_fullscreen(std::u8string title,
																			 struct_handle<GLFWwindow> share) {
	initialize();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {
		log::warning::minor << "Fail to access monitor data.\n"
			"Created window is in 'windowed' mode instead of 'fullscreen'.\n"
			"Window resolution is set to (1280, 720) instead of monitor size.";
		return create(title, 1280, 720, primary_monitor, share);
	}
	auto video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {
		log::warning::minor << "Fail to obtain video data from a monitor.\n"
			"Window resolution is set to (1280, 720) instead of monitor size.";
		return create(title, 1280, 720, primary_monitor, share);
	} else
		return create(title, size_t(video_mode->width), size_t(video_mode->height),
					  primary_monitor, share);
}
clap::gl::detail::window::object clap::gl::detail::window::create_borderless(std::u8string title,
																			 struct_handle<GLFWwindow> share) {
	initialize();
	auto primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {
		log::warning::minor << "Fail to access monitor data.\n"
			"Created window is in 'windowed' mode instead of 'borderless'.\n"
			"Window resolution is set to (1280, 720) instead of monitor size.";
		return create(title, 1280, 720, primary_monitor, share);
	}
	auto video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {
		log::warning::minor << "Fail to obtain video data from a monitor.\n"
			"Window resolution is set to (1280, 720) instead of monitor size.";
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
	initialize();
	glfwPollEvents();
}
void clap::gl::detail::window::wait_events() {
	initialize();
	glfwWaitEvents();
}
void clap::gl::detail::window::wait_events(double seconds) {
	initialize();
	glfwWaitEventsTimeout(seconds);
}
clap::gl::detail::window::object::~object() {
	if (event_handler)
		remove_event_handler();
	if (handle) {
		glfwDestroyWindow(handle);
		log::message::major << "Destroy a " << *this << ".";
	}
}
void clap::gl::detail::window::object::swap_buffers() {
	glfwSwapBuffers(handle);
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
		static void initialize(GLFWwindow *object) {

		}

		static void push(GLFWwindow *object, event::handler_interface *event_handler) {
			if (!object) {
				clap::log::warning::critical << "Fail to enable event handling.\n";
				clap::log::info::major << "'nullptr' was passed instead of a window.";
			} else if (!event_handler) {
				clap::log::warning::critical << "Fail to enable event handling.\n";
				clap::log::info::major << "'nullptr' was passed instead of a event_handler.";
			} else
				mapping.insert(std::pair(object, event_handler));
		}
		static void pop(GLFWwindow *object) {
			if (!object) {
				clap::log::warning::critical << "Fail to disable event handling.\n";
				clap::log::info::major << "'nullptr' was passed instead of a window.";
			} else
				if (auto iterator = mapping.find(object); iterator != mapping.end())
					mapping.erase(iterator);
		}
	public:
		static void on_key(GLFWwindow *w, int key_raw, int scancode_raw, int action_raw, int modificators_raw) {
			auto key = clap::gl::detail::convert::to_key(key_raw);
			auto action = clap::gl::detail::convert::to_key_action(action_raw);
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_key(key, scancode_raw, action, modificators)) {
					clap::log::warning::minor << "Ignore a key event: \'"
						<< modificators << key << " was " << action << "\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a key event: \'"
					<< modificators << key << " was " << action << "\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_character(GLFWwindow *w, unsigned int codepoint_raw) {
			auto codepoint = static_cast<char32_t>(codepoint_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw)) {
					clap::log::warning::minor << "Ignore a character event: \'"
						<< codepoint << "\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a character event: \'"
					<< codepoint << "\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_character(GLFWwindow *w, unsigned int codepoint_raw, int modificators_raw) {
			auto codepoint = static_cast<char32_t>(codepoint_raw);
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw)) {
					clap::log::warning::minor << "Ignore a character event: \'"
						<< modificators << codepoint << "\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a character event: \'"
					<< modificators << codepoint << "\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_mouse_button(GLFWwindow *w, int button_raw, int action_raw, int modificators_raw) {
			auto button = clap::gl::detail::convert::to_mouse_button(button_raw);
			auto action = clap::gl::detail::convert::to_mouse_button_action(action_raw);
			auto modificators = clap::gl::detail::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_mouse_button(button, action, modificators)) {
					clap::log::warning::minor << "Ignore a mouse event: \'"
						<< modificators << button << " was " << action << "\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a mouse event: \'"
					<< modificators << button << " was " << action << "\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_mouse_move(GLFWwindow *w, double x, double y) {
			if (auto handler = get(w); handler) {
				if (!handler->on_mouse_move(x, y)) {
					clap::log::warning::minor << "Ignore a mouse event: \'"
						"Move to (" << x << ", " << y << ")\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a mouse event: \'"
					"Move to (" << x << ", " << y << ")\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_mouse_hover(GLFWwindow *w, int enter_flag_raw) {
			if (auto handler = get(w); handler) {
				if (enter_flag_raw) {
					if (!handler->on_mouse_entering()) {
						clap::log::warning::minor << "Ignore a mouse event: \'"
							"Enter the window\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
				} else
					if (!handler->on_mouse_leaving()) {
						clap::log::warning::minor << "Ignore a mouse event: \'"
							"Leave the window\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
			} else {
				clap::log::warning::minor << "Ignore mouse events: \'"
					"Enter the window\' and \'Leave the window\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_scroll(GLFWwindow *w, double dx, double dy) {
			if (auto handler = get(w); handler) {
				if (!handler->on_scroll(dx, dy)) {
					clap::log::warning::minor << "Ignore a scroll event: \'"
						"Scroll for (" << dx << ", " << dy << ")\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a scroll event: \'"
					"Scroll for (" << dx << ", " << dy << ")\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_window_resize(GLFWwindow *w, int x_raw, int y_raw) {
			size_t x = x_raw, y = y_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_window_resize(x, y)) {
					clap::log::warning::minor << "Ignore a window event: \'"
						"Resize to (" << x << ", " << y << ")\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a window event: \'"
					"Resize to (" << x << ", " << y << ")\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_file_drop(GLFWwindow *w, int count, char const **paths_raw) {
			auto paths = (char8_t const **) paths_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_file_drop(count, paths)) {
					clap::log::warning::minor << "Ignore a file drop event.";
					clap::log::info::major << "Event handler returned 'false'.";
					clap::log::info::major << "File count: " << count;
					for (int i = 0; i < count; i++)
						clap::log::info::minor << i << ": '" << paths[i] << "'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a file drop event.";
				clap::log::info::major << "Handler for this event type isn't installed.";
				clap::log::info::major << "File count: " << count;
				for (int i = 0; i < count; i++)
					clap::log::info::minor << i << ": '" << paths[i] << "'.";
			}
		}

		static void on_window_move(GLFWwindow *w, int x_raw, int y_raw) {
			size_t x = x_raw, y = y_raw;
			if (auto handler = get(w); handler) {
				if (!handler->on_window_move(x, y)) {
					clap::log::warning::minor << "Ignore a window event: \'"
						"Move to (" << x << ", " << y << ")\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a window event: \'"
					"Move to (" << x << ", " << y << ")\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_window_close(GLFWwindow *w) {
			if (auto handler = get(w); handler) {
				if (!handler->on_window_close()) {
					clap::log::warning::minor << "Ignore a window event: \'Close\'.";
					clap::log::info::major << "Event handler returned 'false'.";
				}
			} else {
				clap::log::warning::minor << "Ignore a window event: \'Close\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_window_focus(GLFWwindow *w, int focus_flag_raw) {
			if (auto handler = get(w); handler) {
				if (focus_flag_raw) {
					if (!handler->on_window_focus_gain()) {
						clap::log::warning::minor << "Ignore a window event: \'Gain Focus\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
				} else
					if (!handler->on_window_focus_loss()) {
						clap::log::warning::minor << "Ignore a window event: \'Lose Focus\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
			} else {
				clap::log::warning::minor << "Ignore window events: \'"
					"Gain Focus\' and \'Lose Focus\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_window_minimize(GLFWwindow *w, int flag_raw) {
			if (auto handler = get(w); handler) {
				if (flag_raw) {
					if (!handler->on_window_minimize()) {
						clap::log::warning::minor << "Ignore a window event: \'Minimize\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
				} else
					if (!handler->on_restore_minimized()) {
						clap::log::warning::minor << "Ignore a window event: \'Restore minimized\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
			} else {
				clap::log::warning::minor << "Ignore window events: \'"
					"Minimize\' and \'Restore minimized\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
		static void on_window_maximize(GLFWwindow *w, int flag_raw) {
			if (auto handler = get(w); handler) {
				if (flag_raw) {
					if (!handler->on_window_maximize()) {
						clap::log::warning::minor << "Ignore a window event: \'Maximize\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
				} else
					if (!handler->on_restore_maximized()) {
						clap::log::warning::minor << "Ignore a window event: \'Restore Maximized\'.";
						clap::log::info::major << "Event handler returned 'false'.";
					}
			} else {
				clap::log::warning::minor << "Ignore window events: \'"
					"Maximize\' and \'Restore Maximized\'.";
				clap::log::info::major << "Handler for this event type isn't installed.";
			}
		}
	};
}
std::unordered_map<GLFWwindow *, clap::gl::detail::window::event::handler_interface *> clap::gl::detail::window::event::detail::impl::mapping = {};

void initialize_event_handling(GLFWwindow *window_handle) {
	glfwSetKeyCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_key);
	glfwSetMouseButtonCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_mouse_button);
	glfwSetCharCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_character);
	glfwSetCharModsCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_character);
	glfwSetCursorEnterCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_mouse_hover);
	glfwSetCursorPosCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_mouse_move);
	glfwSetDropCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_file_drop);
	glfwSetScrollCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_scroll);
	glfwSetFramebufferSizeCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_resize);
	glfwSetWindowPosCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_move);
	glfwSetWindowCloseCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_close);
	glfwSetWindowFocusCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_focus);
	glfwSetWindowIconifyCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_minimize);
	glfwSetWindowMaximizeCallback(window_handle, clap::gl::detail::window::event::detail::impl::on_window_maximize);
}

void clap::gl::detail::window::object::set_event_handler(clap::gl::detail::window::event::handler_interface *handler_interface) {
	if (handler_interface) {
		clap::gl::detail::window::event::detail::impl::push(*handle, handler_interface);
		event_handler = handler_interface;
	} else
		if (event_handler) {
			clap::gl::detail::window::event::detail::impl::pop(*handle);
			event_handler = nullptr;
		}
}
