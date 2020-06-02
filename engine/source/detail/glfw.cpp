#include "detail/glfw.hpp"

#include "glfw/glfw3.h"

#include "error.hpp"

bool engine::detail::glfw::is_initialized = false;
bool engine::detail::glfw::is_context_selected = false;

void engine::detail::glfw::initialize_implicitly() {
	error::info(std::string("Initializing glfw. Version: ") + glfwGetVersionString());
	if (!glfwInit())
		error::critical("GLFW initialization error.");
	is_initialized = true;
}

void engine::detail::glfw::terminate_implicitly() {
	glfwTerminate();
	is_initialized = false;
}

void engine::detail::glfw::initialize() {
	if (!is_initialized)
		initialize_implicitly();
	else
		error::warn("GLFW is already initialized.");
}

void engine::detail::glfw::terminate() {
	if (is_initialized)
		terminate_implicitly();
	else
		error::warn("GLFW wasn't initialized, no need to terminate.");
}

engine::detail::glfw_window engine::detail::glfw::create_window(size_t width, size_t height,
																std::string title,
																detail::glfw_monitor_handle monitor,
																detail::glfw_window_handle share) {
	ensure_initialization();

	int signed_width = int(width);
	int signed_height = int(height);
	if (width != size_t(signed_width) || height != size_t(signed_height))
		error::warn("Value overflow. Window dimentions are too large.");

	if (auto out = glfwCreateWindow(signed_width, signed_height, title.c_str(), monitor, share); out)
		return engine::detail::detail::glfw_window_handle(out);
	else
		error::critical("Unable to initialize GLFWwindow. glfwCreateWindow returns a nullptr.");
}
engine::detail::glfw_window engine::detail::glfw::create_window_windowed(size_t width, size_t height,
																		 std::string title,
																		 detail::glfw_window_handle share) {
	return create_window(width, height, title, nullptr, share);
}
engine::detail::glfw_window engine::detail::glfw::create_window_fullscreen(size_t width, size_t height,
																		   std::string title,
																		   detail::glfw_window_handle share) {
	return create_window(width, height, title, primary_monitor(), share);
}
engine::detail::glfw_window engine::detail::glfw::create_window_fullscreen(std::string title,
																		   detail::glfw_window_handle share) {
	ensure_initialization();

	auto video_mode = primary_monitor_video_mode();
	return create_window_fullscreen(size_t(video_mode->width), size_t(video_mode->height), title, share);
}
engine::detail::glfw_window engine::detail::glfw::create_window_borderless(std::string title,
																		   detail::glfw_window_handle share) {
	auto video_mode = glfw::primary_monitor_video_mode();

	glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);

	return glfw::create_window(size_t(video_mode->width), size_t(video_mode->height),
							   title.c_str(), primary_monitor(), share);
}

void engine::detail::glfw::poll_events() {
	ensure_initialization();
	glfwPollEvents();
}

void engine::detail::glfw::wait_events() {
	ensure_initialization();
	glfwWaitEvents();
}

engine::detail::detail::glfw_monitor_handle engine::detail::glfw::primary_monitor() {
	ensure_initialization();

	if (auto out = glfwGetPrimaryMonitor(); out)
		return out;
	else
		error::critical("Unable to access monitor data.");
}

engine::detail::glfw_video_mode const engine::detail::glfw::video_mode(detail::glfw_monitor_handle monitor) {
	ensure_initialization();

	if (auto out = glfwGetVideoMode(monitor); out)
		return detail::glfw_const_video_mode_handle(out);
	else
		error::critical("Unable to obtain video data from a monitor.");
}

void engine::detail::glfw_window::destroy() {
	glfw::ensure_initialization();
	glfwDestroyWindow(handle);
}
void engine::detail::glfw_window::update() {
	glfw::ensure_initialization();
	glfwSwapBuffers(handle);
}
bool engine::detail::glfw_window::should_close() {
	glfw::ensure_initialization();
	return glfwWindowShouldClose(handle);
}
void engine::detail::glfw_window::make_current() {
	glfw::ensure_initialization();
	glfwMakeContextCurrent(handle);
	glfw::is_context_selected = true;
}

size_t engine::detail::glfw_window::width() {
	glfw::ensure_initialization();
	int out;
	glfwGetFramebufferSize(handle, &out, nullptr);
	return size_t(out);
}
size_t engine::detail::glfw_window::height() {
	glfw::ensure_initialization();
	int out;
	glfwGetFramebufferSize(handle, nullptr, &out);
	return size_t(out);
}

size_t engine::detail::glfw_video_mode::width() const { return handle->width; }
size_t engine::detail::glfw_video_mode::height() const { return handle->height; }
int engine::detail::glfw_video_mode::red_bits() const { return handle->redBits; }
int engine::detail::glfw_video_mode::green_bits() const { return handle->greenBits; }
int engine::detail::glfw_video_mode::blue_bits() const { return handle->blueBits; }
int engine::detail::glfw_video_mode::refresh_rate() const { return handle->refreshRate; }
