#include "device_queue.h"

fngn_vk::device_queue::device_queue(const logical_device& logical_device)
	: m_logical_device(logical_device)
{
	vkGetDeviceQueue(
		m_logical_device.vk_device(),
			m_logical_device
				.underlying_physical()
					.get_available_queue_families()
						.graphics_family.value(),
		0,
		&m_queue);
}