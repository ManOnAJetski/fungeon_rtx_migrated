#include "physical_device.h"

fngn_vk::physical_device::physical_device(const VkPhysicalDevice& device)
	: m_device(device)
{
	vkGetPhysicalDeviceProperties(m_device, &m_device_props);
	vkGetPhysicalDeviceFeatures(device, &m_device_features);
}

bool fngn_vk::physical_device::is_suitable() const
{
	bool suitable = true;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(m_device, &deviceFeatures);

	suitable &= deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
	suitable &= get_available_queue_families().graphics_family.has_value();

	return suitable;
}

const fngn_vk::physical_device::queue_family_indicies fngn_vk::physical_device::get_available_queue_families() const
{
	queue_family_indicies family;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; i++)
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			family.graphics_family = i;

	return family;
}
