#include "window.hpp"
#include "essential/log.hpp"
#include <iostream>

int main() {
	engine::logger().add_stream(std::cerr,
								engine::logger_mask::error_every |
								engine::logger_mask::warning_minor |
								engine::logger_mask::message_major |
								engine::logger_mask::info_minor
	);
	engine::logger().add_file("",
							  engine::logger_mask::error_every |
							  engine::logger_mask::warning_minor |
							  engine::logger_mask::message_minor |
							  engine::logger_mask::info_minor
	);

	engine::logger().add_file("full/", engine::logger_mask::every);
	engine::logger().add_file("short/",
							  engine::logger_mask::error_every |
							  engine::logger_mask::warning_major |
							  engine::logger_mask::message_major |
							  engine::logger_mask::info_major
	);

	window w("rrl", window_mode::windowed, 1280, 720);
	return w.loop();
}