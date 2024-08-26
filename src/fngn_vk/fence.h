#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class logical_device;

	class fence
	{
	public:
		PREVENT_COPY(fence);

		fence(const logical_device& device);
		~fence();

		inline const VkFence vk_handle() const { return m_fence; }

	private:
		const logical_device& m_device;
		VkFence m_fence;
	};
}