#include "image.h"

fngn_vk::image::image(
	const logical_device& device,
	VkExtent2D extent,
	VkFormat format,
	std::optional<VkImageTiling> tiling,
	std::optional<VkImageUsageFlags> usage)
	: m_device(device), m_extent(extent), m_format(format), m_imageLayout(VK_IMAGE_LAYOUT_UNDEFINED)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = extent.width;
	imageInfo.extent.height = extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling.value_or(VK_IMAGE_TILING_OPTIMAL);
	imageInfo.initialLayout = m_imageLayout;
	imageInfo.usage = usage.value_or(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	fngn_vk::fnvk_verify(vkCreateImage(device.vk_handle(), &imageInfo, nullptr, &m_image), "create image");
}

fngn_vk::image::image(image&& other)
	: m_device(other.m_device),
	m_extent(other.m_extent),
	m_format(other.m_format),
	m_imageLayout(other.m_imageLayout),
	m_image(other.m_image)
{
}

fngn_vk::image::~image()
{
	if (m_image)
	{
		vkDestroyImage(m_device.vk_handle(), m_image, nullptr);
		m_image = nullptr;
	}
}
