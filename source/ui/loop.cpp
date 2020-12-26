#include "precompiled/ui.hpp"

using namespace std::literals;

inline void zone_loop(clap::ui::zone &zone, vkfw::Window &window) {
	clap::log << cL::message << cL::important << "clap"_tag << "ui"_tag << "zone_loop"_tag
		<< "Start a zone loop for " << zone.name();
	if (zone.on_initialize)
		zone.on_initialize();

	auto last_frame = std::chrono::high_resolution_clock::now();
	while (!window.shouldClose()) {
		constexpr static auto frame_limit = 1'000'000us / 60;

		auto start_time = std::chrono::high_resolution_clock::now();
		auto timestep = std::chrono::duration_cast<clap::utility::timestep>(start_time - last_frame);
		last_frame = start_time;

		if (zone.on_update)
			if (zone.on_update(timestep))
				window.swapBuffers();

		auto end_time = std::chrono::high_resolution_clock::now();
		auto frame_duration = end_time - start_time;

		auto limit_time = start_time + frame_limit;
		std::this_thread::sleep_until(limit_time);

		float numeric_frame_duration =
			std::chrono::duration_cast<std::chrono::duration<float>>(frame_duration).count();
		float numeric_frame_limit =
			std::chrono::duration_cast<std::chrono::duration<float>>(frame_limit).count();

		clap::log << cL::message << cL::negligible << "clap"_tag << "ui"_tag << "zone_loop"_tag
			<< "Draw a frame." << cL::extra
			<< "It took " << numeric_frame_duration << "/" << numeric_frame_limit
			<< "s (" << (100.f * numeric_frame_duration / numeric_frame_limit) << "%)";
	}
}

int clap::ui::loop(int argc, char **argv) {
	{
		auto log_entry = clap::log.add_entry();
		log_entry << cL::message << cL::important << "clap"_tag << "ui"_tag << "loop"_tag
			<< "Start main thread loop." << cL::extra << "Passed arguments (" << argc << "):";
		for (int i = 0; i < argc; i++)
			log_entry << "\n    " << i << ": " << argv[i] << ".";
	}

	std::vector<std::thread> threads;
	detail::manager::zone_loop = [&threads](zone *zone_ptr, vkfw::Window *window_ptr) {
		if (!zone_ptr)
			clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "zone_loop"_tag
			<< "Fail to start zone loop. Received 'ui::zone' is nullptr.";
		else if (!window_ptr)
			clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "zone_loop"_tag
			<< "Fail to start zone loop. Received 'vkfw::Window' is nullptr.";
		else
			threads.emplace_back([zone_ptr, window_ptr] {
				zone_loop(*zone_ptr, *window_ptr);
				detail::manager::remove(*zone_ptr);
			});
	};

	while (!detail::manager::empty()) {
		detail::manager::update();
		vkfw::waitEvents();
	}

	for (auto &thread : threads)
		thread.join();
	threads.clear();

	return 0;
}