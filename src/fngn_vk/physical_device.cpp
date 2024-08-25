#include "physical_device.h"
#include <set>

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
	suitable &= check_device_extension_support();

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

const void fngn_vk::physical_device::print_device_info() const
{
	std::cout << "Physical Device: " << std::endl
		<< "Device Name: " << m_device_props.deviceName << std::endl
		<< "Device Type: " << m_device_props.deviceType << std::endl
		<< "Driver Version: " << m_device_props.driverVersion << std::endl;

	std::cout << "Supported extensions: ";

	for (auto& extension : get_available_extensions())
	{
		std::cout << extension.extensionName << '\t';
	}
}

const std::vector<VkExtensionProperties> fngn_vk::physical_device::get_available_extensions() const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, availableExtensions.data());

	return availableExtensions;
}

const bool fngn_vk::physical_device::check_device_extension_support() const
{
	std::set<std::string> requiredExtensions(m_required_extensions.begin(), m_required_extensions.end());

	for (const auto& extension : get_available_extensions()) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
