#include "precompiled/triangle.hpp"

void initialize_logger() {
	clap::logger::add(std::cout,
					  clap::logger::mask::every_error |
					  clap::logger::mask::minor_warnings |
					  clap::logger::mask::major_messages |
					  clap::logger::mask::every_custom_entry);
	clap::logger::add("problems/",
					  clap::logger::mask::every_error |
					  clap::logger::mask::minor_warnings |
					  clap::logger::mask::no_messages |
					  clap::logger::mask::no_custom_entries);
	clap::logger::add("full/", clap::logger::mask::everything);
	clap::logger::add("tagged/", clap::logger::mask::everything).with_tags();
}

int main(int argc, char **argv) {
	initialize_logger();

	clap::ui::zone z("Triangle example", 1280, 720);

	z.on_initialize = [&z]() {
		clap::log << cL::message << cL::major
			<< "Initialize custom zone: " << z.name();
	};
	z.on_update = [](auto) {
		std::this_thread::sleep_for(4ms);
		return false;
	};
	return clap::ui::loop(argc, argv);
}