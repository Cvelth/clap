#include "window.hpp"

int window::loop() {
	initialize();

	while (!should_close()) {
		render();
		poll_events();
	}
	cleanup();

	return 0;
}

void window::initialize() {}

void window::render() {}

void window::cleanup() {}
