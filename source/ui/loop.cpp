#include "ui/loop.hpp"
#include "ui/detail/glfw.hpp"
#include "ui/detail/state.hpp"

#include <chrono>
#include <thread>

#include "nowide/args.hpp"

#include "essential/log.hpp"

int clap::ui::loop(int argc, char **argv) {
    nowide::args _(argc, argv);
    log::message::minor << "Passed arguments (" << argc << "):";
    for (int i = 0; i < argc; i++)
        log::info::critical << i << ": " << argv[i] << ".";

    //Initialization
    
    log::message::major << "Initialization is done. Starning...";
    while (!detail::state::should_close()) {
        detail::glfw::poll_events();

        auto iteration_end_point = std::chrono::high_resolution_clock::now() 
            + std::chrono::milliseconds(size_t(1000.f / 30));

        //Loop body


        std::this_thread::sleep_until(iteration_end_point);
    }
    log::message::major << "The loop is over. Cleaning up...";

    //Clean up
    detail::glfw::terminate();

    return 0;
}
