#include "physical_device.h"
#include <set>
#include <algorithm>

fngn_vk::physical_device::physical_device(const VkPhysicalDevice& device)
	: m_device(device)
{
	vkGetPhysicalDeviceProperties(m_device, &m_props);
	vkGetPhysicalDeviceFeatures(device, &m_features);
	auto extensions = get_available_extensions();
	std::transform(
		STD_RANGE(extensions),
		std::back_inserter(m_enabled_extension_names),
		[](const VkExtensionProperties& extension) { return extension.extensionName; });
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

	// Cant check swap chain without extension
	if (suitable)
	{
		auto swap_chain_details = query_swap_chain_support(surface);
		suitable &= !swap_chain_details.formats.empty();
		suitable &= !swap_chain_details.present_modes.empty();
	}

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
		<< "Device Name: " << m_props.deviceName << std::endl
		<< "Device Type: " << m_props.deviceType << std::endl
		<< "Driver Version: " << m_props.driverVersion << std::endl;

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


const fngn_vk::physical_device::swap_chain_details fngn_vk::physical_device::query_swap_chain_support(const fngn_vk::surface& surface) const
{
	swap_chain_details details;

	VkSurfaceCapabilitiesKHR capabilities{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device, surface.vk_surface(), &capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, surface.vk_surface(), &format_count, nullptr);
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, surface.vk_surface(), &format_count, details.formats.data());

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, surface.vk_surface(), &present_mode_count, nullptr);
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, surface.vk_surface(), &present_mode_count, details.present_modes.data());

	return details;
}
