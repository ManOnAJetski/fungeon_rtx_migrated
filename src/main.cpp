#include <iostream>
#include <fngn_vk/window.h>
#include <fngn_vk/instance.h>
#include <fngn_vk/physical_devices.h>
#include <fngn_vk/physical_device.h>
#include <fngn_vk/logical_device.h>
#include <fngn_vk/surface.h>
#include <fngn_vk/renderer.h>
#include <fngn_vk/render_pass.h>
#include <fngn_vk/swap_chain.h>
#include <fngn_vk/pipeline_layout.h>
#include <fngn_vk/shader.h>
#include <fngn_vk/graphics_pipeline.h>
#include <fngn_vk/command_pool.h>
#include <fngn_vk/command_buffer.h>
#include <fngn_vk/logical_device.h>
#include <fngn_vk/physical_device.h>
#include <fngn_vk/frame_buffer.h>
#include <fngn_vk/fence.h>
#include <fngn_vk/binary_semaphore.h>
#include <memory>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main(int argc, const char** argv)
{
    fngn_vk::window main_window;
    
    auto ret_code = EXIT_SUCCESS;

    try {
        fngn_vk::instance instance(main_window);
        auto available_devices = fngn_vk::physical_devices::get_phyiscal_devices(instance);
        fngn_vk::physical_devices::print_devices(instance, available_devices);
        fngn_vk::surface surface(instance);

        // TODO: Refactor logical_device to be a shared pointer so that we delete things properly
        fngn_vk::logical_device device(available_devices.front(), surface);

        auto renderer = std::make_unique<fngn_vk::renderer>(device);
        main_window.attach(std::move(renderer));

        main_window.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        ret_code =  EXIT_FAILURE;
    }

    do
    {
        std::cout << '\n' << "Press a key to continue...";
    } while (std::cin.get() != '\n');

    return ret_code;
}