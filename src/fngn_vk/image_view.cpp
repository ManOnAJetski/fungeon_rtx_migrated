#include "image_view.h"
#include "logical_device.h"

fngn_vk::image_view::image_view(const logical_device& device, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags)
	: m_device(device), m_image(image), m_format(format)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = aspect_flags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	fngn_vk::fnvk_verify(vkCreateImageView(m_device.vk_handle(), &createInfo, nullptr, &m_image_view), "create image view");
}

fngn_vk::image_view::~image_view()
{
	if (m_image_view)
	{
		vkDestroyImageView(m_device.vk_handle(), m_image_view, nullptr);
		m_image_view = nullptr;
	}
}
