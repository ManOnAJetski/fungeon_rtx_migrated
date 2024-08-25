#include "physical_device.h"

fngn_vk::physical_device::physical_device(const VkPhysicalDevice& device)
	: m_device(device)
{
	vkGetPhysicalDeviceProperties(m_device, &m_device_props);
	vkGetPhysicalDeviceFeatures(device, &m_device_features);
}

bool fngn_vk::physical_device::is_suitable_for(const fngn_vk::surface& surface) const
{
	bool suitable = true;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(m_device, &deviceFeatures);

	suitable &= deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
	suitable &= get_available_queue_families(surface).is_complete();

	return suitable;
}

const fngn_vk::physical_device::queue_family_indicies fngn_vk::physical_device::get_available_queue_families(const fngn_vk::surface& surface) const
{
	queue_family_indicies family;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			family.graphics_family = i;

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_device, i, surface.vk_surface(), &present_support);

		if (present_support)
			family.present_family = i;
	}

	return family;
}
