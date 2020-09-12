#include "ui/loop.hpp"
#include "ui/detail/glfw.hpp"

int clap::ui::loop(int argc, char **argv) {
    //Initialization

    while (true) {
        //Loop body
    }

    //Clean up
    detail::glfw::terminate();

    return 0;
}
