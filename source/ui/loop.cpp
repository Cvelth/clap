#include "ui/loop.hpp"
#include "ui/detail/glfw.hpp"
#include "ui/detail/state.hpp"

#include <chrono>
#include <thread>

#include "nowide/args.hpp"

#include "essential/guard.hpp"
#include "essential/log.hpp"

static constexpr auto iteration_interval = std::chrono::milliseconds(size_t(1000.f / 30));

int clap::ui::loop(int argc, char **argv) {
	nowide::args _(argc, argv);
	log::message::minor << "Passed arguments (" << argc << "):";
	for (int i = 0; i < argc; i++)
		log::info::critical << i << ": " << argv[i] << ".";

	std::vector<std::thread> threads;
	auto on_add_lambda = [&threads](ui::zone *zone, detail::context &context) {
		threads.emplace_back(
			[zone, &context]() {
				{
					clap::ui::detail::glfw::context_guard guard(context.window);
					zone->initialize();
				}
				clap::log::message::critical << "Window for zone '" << zone->get_name() << "' was initialized.";

				auto last_iteration_time_point = std::chrono::high_resolution_clock::now();
				while (!context.window.should_close()) {
					auto current_time_point = std::chrono::high_resolution_clock::now();
					auto iteration_end_point = current_time_point + iteration_interval;
					auto time_step = current_time_point - last_iteration_time_point;
					last_iteration_time_point = current_time_point;
					{
						clap::ui::detail::glfw::context_guard guard(context.window);
						if (zone->draw(time_step))
							context.window.swap_buffers();
					}
					std::this_thread::sleep_until(iteration_end_point);
				}

				{
					clap::ui::detail::glfw::context_guard guard(context.window);
					zone->clean_up();
				}

				clap::log::message::critical << "Window for zone '" << zone->get_name() << "' was cleaned up.";
				clap::ui::detail::state::remove(zone);
			}
		);
	};

	while (!detail::state::should_close()) {
		detail::state::update(on_add_lambda);
		detail::glfw::wait_events();
	}

	for (auto &thread : threads)
		thread.join();
	threads.clear();

	detail::glfw::terminate();

	return 0;
}
