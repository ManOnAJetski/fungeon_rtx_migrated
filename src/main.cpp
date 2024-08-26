#include <iostream>
#include <fngn_vk/window.h>
#include <fngn_vk/instance.h>
#include <fngn_vk/physical_devices.h>
#include <fngn_vk/logical_device.h>
#include <fngn_vk/swap_chain.h>
#include <fngn_vk/surface.h>
#include <fngn_vk/shader.h>
#include <fngn_vk/pipeline_layout.h>
#include <fngn_vk/graphics_pipeline.h>
#include <fngn_vk/render_pass.h>
#include <fngn_vk/frame_buffer.h>
#include <fngn_vk/command_buffer.h>
#include <fngn_vk/command_pool.h>
#include <filesystem>

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

        fngn_vk::logical_device device(available_devices.front(), surface);

        fngn_vk::swap_chain swap_chain(device, surface);

        std::cout << "Current path - " << std::filesystem::current_path().string() << std::endl;

        fngn_vk::shader vert_shader(
            device,
            std::filesystem::path("shaders\\triangle.vert.spv"),
            VK_SHADER_STAGE_VERTEX_BIT);
        fngn_vk::shader frag_shader(
            device,
            std::filesystem::path("shaders\\triangle.frag.spv"),
            VK_SHADER_STAGE_FRAGMENT_BIT);

        fngn_vk::pipeline_layout pipeline_layout(swap_chain);
        fngn_vk::render_pass render_pass(swap_chain);
        fngn_vk::graphics_pipeline graphics_pipeline(
            swap_chain,
            pipeline_layout,
            render_pass,
            std::vector<const fngn_vk::shader*>{ &vert_shader, &frag_shader });

        std::vector<std::unique_ptr<fngn_vk::frame_buffer>> swap_chain_frame_buffers;

        for (const auto& image_view : swap_chain.image_views())
        {
            swap_chain_frame_buffers.emplace_back(
                std::make_unique<fngn_vk::frame_buffer>(
                    render_pass, swap_chain.extents(), image_view));
        }

        fngn_vk::command_pool pool(device);
        fngn_vk::command_buffer cmd_buffer(pool);

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