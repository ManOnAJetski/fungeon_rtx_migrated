#include "render_pass.h"
#include "swap_chain.h"
#include "logical_device.h"
#include "command_buffer.h"
#include "frame_buffer.h"
#include "graphics_pipeline.h"

fngn_vk::render_pass::render_pass(const swap_chain& swap_chain)
	: m_swap_chain(swap_chain)
{
	VkAttachmentDescription color_attachment{};
	color_attachment.format = m_swap_chain.surface_format().format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;

	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref{};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkRenderPassCreateInfo render_pass_info{};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;

	fngn_vk::fnvk_verify(vkCreateRenderPass(m_swap_chain.device().vk_handle(), &render_pass_info, nullptr, &m_pass), "Create render pass");
}

fngn_vk::render_pass::~render_pass()
{
	if (m_pass)
	{
		vkDestroyRenderPass(m_swap_chain.device().vk_handle(), m_pass, nullptr);
		m_pass = nullptr;
	}
}

const fngn_vk::swap_chain& fngn_vk::render_pass::get_swap_chain() const
{
	return m_swap_chain;
}

void fngn_vk::render_pass::execute(
	const command_buffer& command_buffer,
	const frame_buffer& frame_buffer,
	const graphics_pipeline& graphics_pipeline) const
{
	command_buffer.record();
	{
		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = m_pass;
		render_pass_info.framebuffer = frame_buffer.vk_handle();

		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = m_swap_chain.extents();

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		render_pass_info.clearValueCount = 1;
		render_pass_info.pClearValues = &clearColor;

		vkCmdBeginRenderPass(command_buffer.vk_handle(), &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(command_buffer.vk_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline.vk_handle());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_swap_chain.extents().width);
		viewport.height = static_cast<float>(m_swap_chain.extents().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer.vk_handle(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_swap_chain.extents();
		vkCmdSetScissor(command_buffer.vk_handle(), 0, 1, &scissor);

		vkCmdDraw(command_buffer.vk_handle(), 3, 1, 0, 0);

		vkCmdEndRenderPass(command_buffer.vk_handle());
	}
	command_buffer.end();
}
