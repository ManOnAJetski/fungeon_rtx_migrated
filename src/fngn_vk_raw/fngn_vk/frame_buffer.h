#pragma once
#include <fngn_vk/base.h>
#include <vector>
#include <memory>

namespace fngn_vk
{
	class render_pass;
	class logical_device;
	class image_view;

	class frame_buffer
	{
	public:
		PREVENT_COPY(frame_buffer);

		frame_buffer(const render_pass& pass, const VkExtent2D& extents, const std::unique_ptr<image_view>& image_view);
		~frame_buffer();

		inline const VkFramebuffer vk_handle() const { return m_buffer; }

	private:
		const logical_device& m_device;
		VkFramebuffer m_buffer{};
	};
}