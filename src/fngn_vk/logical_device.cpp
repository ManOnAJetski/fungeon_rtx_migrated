#include "logical_device.h"
#include <stdexcept>

fngn_vk::logical_device::logical_device(const physical_device& physical_device)
	: m_physical_device(physical_device)
{
	auto indices = m_physical_device.get_available_queue_families();

	if (!indices.graphics_family.has_value())
	{
		throw std::runtime_error("Logical device could not be created, no queue families!");
	}

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphics_family.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &m_physical_device.get_features();

	fnvk_verify(vkCreateDevice(physical_device.vk_physical_device(), &createInfo, nullptr, &m_device), "Logical device creation");
}

fngn_vk::logical_device::~logical_device()
{
	if (m_device != nullptr)
	{
		vkDestroyDevice(m_device, nullptr);
		m_device = nullptr;
	}
}
