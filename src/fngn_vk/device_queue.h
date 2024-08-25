#pragma once
#include <fngn_vk/logical_device.h>

namespace fngn_vk
{
	class device_queue
	{
	public:
		explicit device_queue(const logical_device& logical_device);

	private:
		const logical_device& m_logical_device;
		VkQueue m_queue = VK_NULL_HANDLE;
	};
}