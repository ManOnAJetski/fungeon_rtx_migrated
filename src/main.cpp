#include <iostream>
#include <fngn_vk/window.h>
#include <fngn_vk/instance.h>
#include <fngn_vk/physical_devices.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main(int argc, const char** argv)
{
    fngn_vk::window main_window;
    
    try {
        fngn_vk::instance instance(main_window);
        fngn_vk::physical_devices::print_availabe_devices(instance);
        main_window.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}