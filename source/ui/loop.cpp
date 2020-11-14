#include "ui/loop.hpp"
#include "ui/detail/state.hpp"

#include <chrono>
#include <thread>

#include "nowide/args.hpp"

#include "essential/guard.hpp"
#include "essential/log.hpp"

#include "gl/detail/window.hpp"

#include "resource/resource.hpp"

//Helper types for temporary settings.
enum class context_activation_t {
	never = 0,
	once_per_thread = 1,
	once_per_major_call = 2,
	once_per_call = 3
};

//Temporary settings.
static constexpr auto iteration_interval = std::chrono::milliseconds(size_t(1000.f / 30));
static constexpr auto context_activation = context_activation_t::once_per_thread;

inline static void initialize_impl(clap::ui::zone *zone) {
	zone->initialize();
}
inline static void draw_iteration_impl(clap::ui::zone *zone, 
									   clap::gl::detail::context &context, 
									   std::chrono::nanoseconds const &time_step) {
	if (zone->draw(time_step))
		context.window.swap_buffers();
}
inline static void clean_up_impl(clap::ui::zone *zone) {
	zone->clean_up();
	clap::resource::clear();
}

template <context_activation_t context_activation>
inline static void draw_loop_impl(clap::ui::zone *zone, 
								  clap::gl::detail::context &context) {
	auto last_iteration_time_point = std::chrono::high_resolution_clock::now();
	while (!context.window.should_close()) {
		auto current_time_point = std::chrono::high_resolution_clock::now();
		auto iteration_end_point = current_time_point + iteration_interval;
		auto time_step = current_time_point - last_iteration_time_point;
		last_iteration_time_point = current_time_point;

		if constexpr (context_activation == context_activation_t::once_per_call) {
			auto guard = context.make_current();
			draw_iteration_impl(zone, context, time_step);
		} else
			draw_iteration_impl(zone, context, time_step);
		
		std::this_thread::sleep_until(iteration_end_point);
	}
}

template <context_activation_t context_activation>
inline static void initialize_stage_impl(clap::ui::zone *zone, 
										 clap::gl::detail::context &context) {
	clap::log::message::critical << "Window for zone '" << zone->get_name() << "' was created. Calling 'initialize()'...";

	if constexpr (context_activation == context_activation_t::once_per_major_call || context_activation == context_activation_t::once_per_call) {
		auto guard = context.make_current();
		initialize_impl(zone);
	} else
		initialize_impl(zone);

	clap::log::message::critical << "Window for zone '" << zone->get_name() << "' was initialized.";
}

template <context_activation_t context_activation>
inline static void draw_loop_stage_impl(clap::ui::zone *zone, 
										clap::gl::detail::context &context) {
	if constexpr (context_activation == context_activation_t::once_per_major_call) {
		auto guard = context.make_current();
		draw_loop_impl<context_activation>(zone, context);
	} else
		draw_loop_impl<context_activation>(zone, context);
}

template <context_activation_t context_activation>
inline static void clean_up_stage_impl(clap::ui::zone *zone, 
									   clap::gl::detail::context &context) {
	clap::log::message::critical << "Window for zone '" << zone->get_name() << "' is to be closed. Calling 'clean_up()'...";

	if constexpr (context_activation == context_activation_t::once_per_major_call || context_activation == context_activation_t::once_per_call) {
		auto guard = context.make_current();
		clean_up_impl(zone);
	} else
		clean_up_impl(zone);

	clap::log::message::critical << "Window for zone '" << zone->get_name() << "' was cleaned up.";
}

template <context_activation_t context_activation>
inline static void thread_callable(clap::ui::zone *zone, 
								   clap::gl::detail::context &context) {
	if constexpr (context_activation == context_activation_t::once_per_thread) {
		auto guard = context.make_current();
		initialize_stage_impl<context_activation>(zone, context);
		draw_loop_stage_impl<context_activation>(zone, context);
		clean_up_stage_impl<context_activation>(zone, context);
	} else {
		initialize_stage_impl<context_activation>(zone, context);
		draw_loop_stage_impl<context_activation>(zone, context);
		clean_up_stage_impl<context_activation>(zone, context);
	}
}

int clap::ui::loop(int argc, char **argv) {
	nowide::args _(argc, argv);
	log::message::major << "Main thread loop has started.";
	log::info::major << "Passed arguments (" << argc << "):";
	for (int i = 0; i < argc; i++)
		log::info::major << '\t' << i << ": " << argv[i] << ".";

	clap::impromptu::resource::identify();
	clap::resource::identify();

	std::vector<std::thread> threads;
	auto on_add_lambda = [&threads](ui::zone *zone, gl::detail::context &context) {
		threads.emplace_back(
			[zone, &context]() {
				thread_callable<context_activation>(zone, context);
				clap::ui::detail::state::remove(zone);
			}
		);
	};

	while (!detail::state::should_close()) {
		detail::state::update(on_add_lambda);
		gl::detail::window::wait_events();
	}

	for (auto &thread : threads)
		thread.join();
	threads.clear();

	gl::detail::window::terminate();

	return 0;
}
