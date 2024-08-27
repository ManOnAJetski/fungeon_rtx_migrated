#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class logical_device;

	class binary_semaphore
	{
	public:
		PREVENT_COPY(binary_semaphore)

		binary_semaphore(const logical_device& device);
		~binary_semaphore();
		
		inline const VkSemaphore vk_handle() const { return m_semaphore; }

	private:
		const logical_device& m_device;
		VkSemaphore m_semaphore;
	};
}