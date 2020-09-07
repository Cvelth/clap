#include "essential/log.hpp"
#include "ui.hpp"
#include <iostream>

int main(int argc, char **argv) {
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

	clap::ui::zone empty_zone;
	return clap::ui::loop(argc, argv);
}