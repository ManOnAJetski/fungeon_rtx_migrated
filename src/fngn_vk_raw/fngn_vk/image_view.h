#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class logical_device;

	class image_view
	{
	public:
		PREVENT_COPY(image_view)

		explicit image_view(const logical_device& device, VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);
		~image_view();

		inline const VkImageView vk_handle() const { return m_image_view; }

	private:
		const logical_device& m_device;
		const VkFormat m_format;
		VkImageView m_image_view{};
	};
}