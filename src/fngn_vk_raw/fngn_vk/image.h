#pragma once
#include <fngn_vk/base.h>
#include <optional>

namespace fngn_vk
{
	class logical_device;

	class image
	{
	public:
		image(const image&) = delete;
		image& operator = (const image&) = delete;
		image& operator = (image&&) = delete;

		image(
			const logical_device& device,
			VkExtent2D extent,
			VkFormat format,
			std::optional<VkImageTiling> tiling = std::nullopt,
			std::optional<VkImageUsageFlags> usage = std::nullopt);

		image(image&& other);

		~image();

	private:
		const logical_device& m_device;
		const VkExtent2D m_extent;
		const VkFormat m_format;
		VkImageLayout m_imageLayout;

		VkImage m_image{};
	};
}