#include "window/detail/glfw.hpp"

#include "glfw/glfw3.h"

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
