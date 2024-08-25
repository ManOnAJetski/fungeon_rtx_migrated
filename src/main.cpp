#include <iostream>
#include <fngn_vk/window.h>
#include <fngn_vk/instance.h>
#include <fngn_vk/physical_devices.h>
#include <fngn_vk/logical_device.h>
#include <fngn_vk/swap_chain.h>
#include <fngn_vk/surface.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main(int argc, const char** argv)
{
    fngn_vk::window main_window;
    
    try {
        fngn_vk::instance instance(main_window);
        auto available_devices = fngn_vk::physical_devices::get_phyiscal_devices(instance);
        fngn_vk::physical_devices::print_devices(instance, available_devices);
        fngn_vk::surface surface(instance);

        fngn_vk::logical_device device(available_devices.front(), surface);

        fngn_vk::swap_chain swap_chain(device, surface);

        main_window.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}