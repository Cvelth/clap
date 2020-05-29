#include "window_interface.hpp"
#include "error.hpp"
#include "gl/glfw.hpp"
#include "gl/state.hpp"

engine::window_interface::window_interface(std::string const &title, window_mode mode, 
										   size_t width, size_t height) 
										: aspect_ratio(double(width) / height), handle(nullptr) {
	switch (mode) {
		case window_mode::windowed:
			handle = new glfw_window(glfw::create_window_windowed(width, height, title));
			break;

		case window_mode::fullscreen:
			handle = new glfw_window(glfw::create_window_fullscreen(width, height, title));
			break;

		case window_mode::borderless_window:
			handle = new glfw_window(glfw::create_window_borderless(title));
			break;

		default:
			error::critical("Unsupported window_mode enum value passed on window initialization.");
	}

	handle->make_current();
	gl::state::load();
}

engine::window_interface::window_interface(std::string const &title, window_mode mode) 
	: window_interface(title, mode, glfw::primary_monitor_video_mode().width(),
					   glfw::primary_monitor_video_mode().height()) {
	if (mode == window_mode::windowed)
		error::warn("It's recommended to implicitly set width and height when starting in windowed mode.");
}

engine::window_interface::~window_interface() {
	if (handle) {
		handle->destroy();
		delete handle;
	}
}

void engine::window_interface::update() {
	handle->update();
}

void engine::window_interface::poll_events() {
	glfw::poll_events();
}

void engine::window_interface::wait_events() {
	glfw::wait_events();
}

size_t engine::window_interface::width() {
	return handle->width();
}

size_t engine::window_interface::height() {
	return handle->height();
}

bool engine::window_interface::should_close() {
	return handle->should_close();
}

int engine::window_interface::loop() {
	initialize();
	while (!should_close()) {
		render();
		update();
	}
	cleanup();

	return 0;
}
