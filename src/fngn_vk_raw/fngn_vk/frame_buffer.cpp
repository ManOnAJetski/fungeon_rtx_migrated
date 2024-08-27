#include "frame_buffer.h"
#include "logical_device.h"
#include "render_pass.h"
#include "swap_chain.h"
#include "image_view.h"
#include <algorithm>
#include <iterator>

fngn_vk::frame_buffer::frame_buffer(
	const render_pass& pass,
	const VkExtent2D& extents,
	const std::unique_ptr<image_view>& image_view)
	: m_device(pass.get_swap_chain().device())
{
	std::vector<VkImageView> vk_image_views;
	vk_image_views.emplace_back(image_view->vk_handle());

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = pass.vk_handle();
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = vk_image_views.data();
	framebufferInfo.width = pass.get_swap_chain().extents().width;
	framebufferInfo.height = pass.get_swap_chain().extents().height;
	framebufferInfo.layers = 1;

	fngn_vk::fnvk_verify(
		vkCreateFramebuffer(m_device.vk_handle(), &framebufferInfo, nullptr, &m_buffer),
		"Create frame buffer");
}

fngn_vk::frame_buffer::~frame_buffer()
{
	if (m_buffer)
	{
		vkDestroyFramebuffer(m_device.vk_handle(), m_buffer, nullptr);
		m_buffer = nullptr;
	}
}
