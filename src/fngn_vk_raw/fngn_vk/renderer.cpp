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
                std::filesystem::absolute(std::filesystem::path("shaders\/triangle.vert.spv")),
                VK_SHADER_STAGE_VERTEX_BIT)).get());

    shader_ptrs.push_back(
        m_shaders.emplace_back(
            std::make_unique<fngn_vk::shader>(
                device,
                std::filesystem::absolute(std::filesystem::path("shaders\/triangle.frag.spv")),
                VK_SHADER_STAGE_FRAGMENT_BIT)).get());

    m_pipeline_layout = std::make_unique<fngn_vk::pipeline_layout>(*m_swap_chain);
    m_render_pass = std::make_unique<fngn_vk::render_pass>(*m_swap_chain);

    m_graphics_pipeline =
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

    m_in_flight_fence = std::make_unique<fngn_vk::fence>(device, VK_FENCE_CREATE_SIGNALED_BIT);
    m_image_available_semaphore = std::make_unique<fngn_vk::binary_semaphore>(device);
    m_render_finished_semaphore = std::make_unique<fngn_vk::binary_semaphore>(device);
}

fngn_vk::renderer::~renderer()
{
    m_render_finished_semaphore.reset();
    m_image_available_semaphore.reset();
    m_in_flight_fence.reset();
    m_command_pool.reset();

    for (auto& buffer : m_swap_chain_frame_buffers)
    {
        buffer.reset();
    }

    m_graphics_pipeline.reset();
    m_pipeline_layout.reset();
    m_render_pass.reset();
    m_swap_chain.reset();
}

void fngn_vk::renderer::draw()
{
    std::vector<VkFence> fences{ m_in_flight_fence->vk_handle() };

    vkWaitForFences(m_swap_chain->device().vk_handle(), 1, fences.data(), VK_TRUE, UINT64_MAX);
    vkResetFences(m_swap_chain->device().vk_handle(), 1, fences.data());

    uint32_t img_idx;

    vkAcquireNextImageKHR(
        m_swap_chain->device().vk_handle(),
        m_swap_chain->vk_handle(),
        UINT64_MAX,
        m_image_available_semaphore->vk_handle(),
        VK_NULL_HANDLE,
        &img_idx);

    vkResetCommandBuffer(m_command_buffer->vk_handle(), 0);

    m_command_buffer->record();
    {
        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = m_render_pass->vk_handle();
        render_pass_info.framebuffer = m_swap_chain_frame_buffers[img_idx]->vk_handle();

        render_pass_info.renderArea.offset = { 0, 0 };
        render_pass_info.renderArea.extent = m_swap_chain->extents();

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        render_pass_info.clearValueCount = 1;
        render_pass_info.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_command_buffer->vk_handle(), &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_command_buffer->vk_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline->vk_handle());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swap_chain->extents().width);
        viewport.height = static_cast<float>(m_swap_chain->extents().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_command_buffer->vk_handle(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_swap_chain->extents();
        vkCmdSetScissor(m_command_buffer->vk_handle(), 0, 1, &scissor);

        vkCmdDraw(m_command_buffer->vk_handle(), 3, 1, 0, 0);

        vkCmdEndRenderPass(m_command_buffer->vk_handle());
    }
    m_command_buffer->end();

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    std::vector<VkCommandBuffer> command_buffers{ m_command_buffer->vk_handle() };

    VkSemaphore waitSemaphores[] = { m_image_available_semaphore->vk_handle() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = waitSemaphores;
    submit_info.pWaitDstStageMask = waitStages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = command_buffers.data();

    VkSemaphore signal_semaphores[] = { m_render_finished_semaphore->vk_handle() };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkQueueSubmit(m_swap_chain->device().graphics_queue(), 1, &submit_info, m_in_flight_fence->vk_handle());

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swapChains[] = { m_swap_chain->vk_handle() };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapChains;
    present_info.pImageIndices = &img_idx;
    present_info.pResults = nullptr; // Optional

    vkQueuePresentKHR(m_swap_chain->device().graphics_queue(), &present_info);
}

void fngn_vk::renderer::wait_for_device()
{
    vkDeviceWaitIdle(m_swap_chain->device().vk_handle());
}
