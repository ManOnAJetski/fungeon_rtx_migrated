#include "logical_device.h"
#include <stdexcept>
#include <set>

fngn_vk::logical_device::logical_device(
	const physical_device& physical_device,
	const surface& surface)
	: m_physical_device(physical_device), m_surface(surface)
{
	auto indices = m_physical_device.get_available_queue_families(m_surface);

	if (!indices.is_complete())
	{
		throw std::runtime_error("Logical device could not be created, no queue families!");
	}

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : unique_queue_families) {
		VkDeviceQueueCreateInfo queue_create_info{};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queueFamily;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queuePriority;
		queue_create_infos.push_back(queue_create_info);
	}

	VkDeviceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());;

	create_info.pEnabledFeatures = &m_physical_device.get_features();

	create_info.enabledExtensionCount = static_cast<uint32_t>(m_physical_device.get_enabled_extension_names().size());
	create_info.ppEnabledExtensionNames = m_physical_device.get_enabled_extension_names().data();

	fnvk_verify(vkCreateDevice(m_physical_device.vk_physical_device(), &create_info, nullptr, &m_device), "Logical device creation");
}

fngn_vk::logical_device::~logical_device()
{
	if (m_device != nullptr)
	{
		vkDestroyDevice(m_device, nullptr);
		m_device = nullptr;
	}
}
