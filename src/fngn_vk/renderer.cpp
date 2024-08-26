#include "renderer.h"
#include "swap_chain.h"
#include "render_pass.h"
#include "pipeline_layout.h"
#include "shader.h"
#include "graphics_pipeline.h"
#include "command_pool.h"
#include "command_buffer.h"
#include "logical_device.h"
#include "frame_buffer.h"
#include "render_pass.h"
#include "fence.h"
#include "binary_semaphore.h"

fngn_vk::renderer::renderer(const logical_device& device)
    : m_swap_chain(std::make_unique<fngn_vk::swap_chain>(device))
{
    std::vector<const fngn_vk::shader*> shader_ptrs;

    shader_ptrs.push_back(
        m_shaders.emplace_back(
            std::make_unique<fngn_vk::shader>(
                device,
                std::filesystem::path("shaders\\triangle.vert.spv"),
                VK_SHADER_STAGE_VERTEX_BIT)).get());

    shader_ptrs.push_back(
        m_shaders.emplace_back(
            std::make_unique<fngn_vk::shader>(
                device,
                std::filesystem::path("shaders\\triangle.frag.spv"),
                VK_SHADER_STAGE_FRAGMENT_BIT)).get());

    m_pipeline_layout = std::make_unique<fngn_vk::pipeline_layout>(*m_swap_chain);
    m_render_pass = std::make_unique<fngn_vk::render_pass>(*m_swap_chain);

    m_graphics_pipline =
        std::make_unique<fngn_vk::graphics_pipeline>(
            *m_swap_chain,
            *m_pipeline_layout,
            *m_render_pass,
            shader_ptrs);

    for (const auto& image_view : m_swap_chain->image_views())
    {
        m_swap_chain_frame_buffers.emplace_back(
            std::make_unique<fngn_vk::frame_buffer>(
                *m_render_pass, m_swap_chain->extents(), image_view));
    }

    m_command_pool = std::make_unique<fngn_vk::command_pool>(device);
    m_command_buffer = std::make_unique<fngn_vk::command_buffer>(*m_command_pool);

    m_in_flight_fence = std::make_unique<fngn_vk::fence>(device);
    m_image_available_semaphore = std::make_unique<fngn_vk::binary_semaphore>(device);
    m_render_finished_semaphore = std::make_unique<fngn_vk::binary_semaphore>(device);
}

void fngn_vk::renderer::draw()
{
}
