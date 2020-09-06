#include "window/detail/glfw.hpp"
#include "window/event_handler_interface.hpp"

#include "glfw/glfw3.h"

#include <unordered_map>

#include "essential/log.hpp"

bool clap::detail::glfw::is_initialized = false;
bool clap::detail::glfw::is_context_selected = false;

void clap::detail::glfw::initialize_implicitly() {
	if (!glfwInit())
		log::warning::critical << "GLFW initialization error.";
	else {
		log::message::major << "GLFW was initialized.";
		log::info::critical << "Version: " << glfwGetVersionString();

		is_initialized = true;
	}
}

void clap::detail::glfw::terminate_implicitly() {
	glfwTerminate();
	log::message::major << "GLFW was terminated.";
	is_initialized = false;
}

void clap::detail::glfw::initialize() {
	if (!is_initialized)
		initialize_implicitly();
	else
		log::warning::minor << "Attempt to initialized GLFW more than once.";
}

void clap::detail::glfw::terminate() {
	if (is_initialized)
		terminate_implicitly();
	else
		log::warning::minor << "Attempt to terminate GLFW when it isn't initialied.";
}

clap::detail::glfw_window clap::detail::glfw::create_window(size_t width, size_t height,
															std::string title,
															detail::glfw_monitor_handle monitor,
															detail::glfw_window_handle share) {
	ensure_initialization();

	int signed_width = int(width);
	int signed_height = int(height);
	if (width != size_t(signed_width) || height != size_t(signed_height)) {
		log::warning::critical << "Window dimention value overflow. They are too large.";
		log::info::major << "Dimentions are: (" << width << ", " << height << ").";
	}

	if (auto out = glfwCreateWindow(signed_width, signed_height, title.c_str(), monitor, share); out) {
		log::message::major << "New window was created with dimentions (" << width << ", " << height << ").";
		return clap::detail::detail::glfw_window_handle(out);
	} else {
		log::error::major << "Unable to initialize GLFWwindow.";
		log::info::major << "glfwCreateWindow returns a nullptr.";
	}
}
clap::detail::glfw_window clap::detail::glfw::create_window_windowed(size_t width, size_t height,
																	 std::string title,
																	 detail::glfw_window_handle share) {
	return create_window(width, height, title, nullptr, share);
}
clap::detail::glfw_window clap::detail::glfw::create_window_fullscreen(size_t width, size_t height,
																	   std::string title,
																	   detail::glfw_window_handle share) {
	return create_window(width, height, title, primary_monitor(), share);
}
clap::detail::glfw_window clap::detail::glfw::create_window_fullscreen(std::string title,
																	   detail::glfw_window_handle share) {
	ensure_initialization();

	auto video_mode = primary_monitor_video_mode();
	return create_window_fullscreen(size_t(video_mode->width), size_t(video_mode->height), title, share);
}
clap::detail::glfw_window clap::detail::glfw::create_window_borderless(std::string title,
																	   detail::glfw_window_handle share) {
	auto video_mode = glfw::primary_monitor_video_mode();

	glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

	return glfw::create_window(size_t(video_mode->width), size_t(video_mode->height),
							   title.c_str(), primary_monitor(), share);
}

void clap::detail::glfw::poll_events() {
	ensure_initialization();
	glfwPollEvents();
}

void clap::detail::glfw::wait_events() {
	ensure_initialization();
	glfwWaitEvents();
}

void clap::detail::glfw::wait_events(double seconds) {
	ensure_initialization();
	glfwWaitEventsTimeout(seconds);
}

clap::detail::detail::glfw_monitor_handle clap::detail::glfw::primary_monitor() {
	ensure_initialization();

	if (auto out = glfwGetPrimaryMonitor(); out)
		return out;
	else
		log::error::minor << "Unable to access monitor data.";
}

clap::detail::glfw_video_mode const clap::detail::glfw::video_mode(detail::glfw_monitor_handle monitor) {
	ensure_initialization();

	if (auto out = glfwGetVideoMode(monitor); out)
		return detail::glfw_const_video_mode_handle(out);
	else
		log::error::minor << "Unable to obtain video data from a monitor.";
}

void clap::detail::glfw_window::destroy() {
	glfw::ensure_initialization();
	glfwDestroyWindow(handle);
	log::message::major << "A window was destroyed.";
}
void clap::detail::glfw_window::update() {
	glfw::ensure_initialization();
	glfwSwapBuffers(handle);
	log::message::negligible << "A window was updated.";
}
bool clap::detail::glfw_window::should_close() {
	glfw::ensure_initialization();
	return glfwWindowShouldClose(handle);
}
void clap::detail::glfw_window::make_current() {
	glfw::ensure_initialization();
	glfwMakeContextCurrent(handle);
	glfw::is_context_selected = true;
	log::message::minor << "The GL context of this window was make current.";
}

size_t clap::detail::glfw_window::width() {
	glfw::ensure_initialization();
	int out;
	glfwGetFramebufferSize(handle, &out, nullptr);
	return size_t(out);
}
size_t clap::detail::glfw_window::height() {
	glfw::ensure_initialization();
	int out;
	glfwGetFramebufferSize(handle, nullptr, &out);
	return size_t(out);
}

size_t clap::detail::glfw_video_mode::width() const { return handle->width; }
size_t clap::detail::glfw_video_mode::height() const { return handle->height; }
int clap::detail::glfw_video_mode::red_bits() const { return handle->redBits; }
int clap::detail::glfw_video_mode::green_bits() const { return handle->greenBits; }
int clap::detail::glfw_video_mode::blue_bits() const { return handle->blueBits; }
int clap::detail::glfw_video_mode::refresh_rate() const { return handle->refreshRate; }

namespace clap::event::detail {
	class impl {
	private:
		static std::unordered_map<GLFWwindow *, clap::event::handler_interface *> mapping;
	protected:
		inline static clap::event::handler_interface *get(GLFWwindow *key) {
			auto ptr = mapping.find(key);
			return ptr != mapping.end() ? ptr->second : nullptr;
		}
	public:
		static void push(GLFWwindow *window_handle, clap::event::handler_interface *event_handler) {
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
			else
				mapping.erase(window_handle);
		}
	public:
		static void on_key(GLFWwindow *w, int key_raw, int scancode_raw, int action_raw, int modificators_raw) {
			auto key = clap::event::detail::convert::to_key(key_raw);
			auto action = clap::event::detail::convert::to_key_action(action_raw);
			auto modificators = clap::event::detail::convert::to_modificator_mask(modificators_raw);
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
			auto modificators = clap::event::detail::convert::to_modificator_mask(modificators_raw);
			if (auto handler = get(w); handler) {
				if (!handler->on_character(codepoint_raw))
					clap::log::warning::minor << "Character event \'" << modificators << codepoint
					<< "\' wasn't handled (handler returned 'false').";
			} else
				clap::log::warning::minor << "Character event \'" << modificators << codepoint
				<< "\' has no handler installed.";
		}
		static void on_mouse_button(GLFWwindow *w, int button_raw, int action_raw, int modificators_raw) {
			auto button = clap::event::detail::convert::to_mouse_button(button_raw);
			auto action = clap::event::detail::convert::to_mouse_button_action(action_raw);
			auto modificators = clap::event::detail::convert::to_modificator_mask(modificators_raw);
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
			auto action = clap::event::detail::convert::to_mouse_hover_action(enter_flag_raw);
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
std::unordered_map<GLFWwindow *, clap::event::handler_interface *> clap::event::detail::impl::mapping = {};

void clap::detail::glfw_window::set_event_handler(clap::event::handler_interface *handler_interface) {
	if (handler_interface) {
		clap::event::detail::impl::push(*handle, handler_interface);

		glfwSetKeyCallback(*handle, clap::event::detail::impl::on_key);
		glfwSetMouseButtonCallback(*handle, clap::event::detail::impl::on_mouse_button);
		glfwSetCharCallback(*handle, clap::event::detail::impl::on_character);
		glfwSetCharModsCallback(*handle, clap::event::detail::impl::on_character);
		glfwSetCursorEnterCallback(*handle, clap::event::detail::impl::on_mouse_hover);
		glfwSetCursorPosCallback(*handle, clap::event::detail::impl::on_mouse_move);
		glfwSetDropCallback(*handle, clap::event::detail::impl::on_file_drop);
		glfwSetScrollCallback(*handle, clap::event::detail::impl::on_scroll);
		glfwSetFramebufferSizeCallback(*handle, clap::event::detail::impl::on_resize);

	} else
		clap::event::detail::impl::pop(*handle);
}