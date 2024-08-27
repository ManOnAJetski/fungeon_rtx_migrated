#include "device_queue.h"
#include "logical_device.h"
#include "physical_device.h"
#include "surface.h"
#include <set>

fngn_vk::device_queue::device_queue(
	const logical_device& logical_device,
	const surface& surface)
	: m_logical_device(logical_device)
{
	auto indices = m_logical_device.underlying_physical()
		.get_available_queue_families(surface);

	vkGetDeviceQueue(
		m_logical_device.vk_handle(),
			m_logical_device
				.underlying_physical()
					.get_available_queue_families(surface)
						.graphics_family.value(),
		0,
		&m_queue);
}