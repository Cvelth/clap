#include "precompiled/ui.hpp"

using namespace std::literals;

int clap::ui::loop(int argc, char **argv) {
	{
		auto log_entry = clap::log.add_entry();
		log_entry << cL::message << cL::important << "clap"_tag << "ui"_tag << "loop"_tag
			<< "Start main thread loop." << cL::extra << "Passed arguments (" << argc << "):";
		for (int i = 0; i < argc; i++)
			log_entry << "\n    " << i << ": " << argv[i] << ".";
	}

	while (!detail::manager::empty()) {
		detail::manager::update();
		vkfw::waitEvents();
	}
	detail::manager::wait();

	return 0;
}