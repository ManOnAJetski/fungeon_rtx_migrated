#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/logical_device.h>

namespace fngn_vk
{
	class image_view
	{
	public:
		PREVENT_COPY(image_view)

		explicit image_view(const logical_device& device, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);
		~image_view();

	private:
		const logical_device& m_device;
		const VkFormat m_format;

		VkImage m_image;
		VkImageView m_image_view{};
	};
}