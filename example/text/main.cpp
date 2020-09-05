#include "window.hpp"
#include "essential/log.hpp"
#include <iostream>

int main() {
	clap::logger().add_stream(std::cerr,
							  clap::logger_mask::error_every |
							  clap::logger_mask::warning_minor |
							  clap::logger_mask::message_major |
							  clap::logger_mask::info_minor
	);
	clap::logger().add_file("",
							clap::logger_mask::error_every |
							clap::logger_mask::warning_minor |
							clap::logger_mask::message_minor |
							clap::logger_mask::info_minor
	);

	clap::logger().add_file("full/", clap::logger_mask::every);
	clap::logger().add_file("short/",
							clap::logger_mask::error_every |
							clap::logger_mask::warning_major |
							clap::logger_mask::message_major |
							clap::logger_mask::info_major
	);

	window w("text example", window_mode::windowed, 1280, 720);
	return w.loop();
}