#include "window/window_interface.hpp"

#include "window/detail/glfw.hpp"
#include "essential/log.hpp"
#include "gl/detail/state.hpp"

engine::window_interface::window_interface(std::string const &title, window_mode mode, 
										   size_t width, size_t height) 
										: aspect_ratio(double(width) / height), handle(nullptr) {
	switch (mode) {
		case window_mode::windowed:
			handle = new detail::glfw_window(detail::glfw::create_window_windowed(width, height, title));
			break;

		case window_mode::fullscreen:
			handle = new detail::glfw_window(detail::glfw::create_window_fullscreen(width, height, title));
			break;

		case window_mode::borderless_window:
			handle = new detail::glfw_window(detail::glfw::create_window_borderless(title));
			break;

		default:
			log::warning::critical << "Unsupported window_mode enum value passed on window initialization.";
			return;
	}

	handle->make_current();
	gl::detail::state::load();
}

engine::window_interface::window_interface(std::string const &title, window_mode mode) 
	: window_interface(title, mode, detail::glfw::primary_monitor_video_mode().width(),
					   detail::glfw::primary_monitor_video_mode().height()) {
	if (mode == window_mode::windowed)
		log::warning::minor << "It's recommended to implicitly set width and height when starting in windowed mode.";
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
	detail::glfw::poll_events();
}

void engine::window_interface::wait_events() {
	detail::glfw::wait_events();
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
	log::message::critical << "Window loop initialization has started.";
	initialize();
	log::message::critical << "Window loop initialization is complete. Starting the loop...";
	while (!should_close()) {
		render();
		update();
	}
	log::message::critical << "Window loop is complete. Cleaning up...";
	cleanup();

	return 0;
}
