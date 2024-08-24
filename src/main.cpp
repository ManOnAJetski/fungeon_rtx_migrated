#include <iostream>
#include <fngn_vk/window.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main(int argc, const char** argv)
{
    fngn_vk::window main_window;

    try {
        main_window.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}