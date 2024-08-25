#include "physical_devices.h"
#include <algorithm>

const std::vector<fngn_vk::physical_device> fngn_vk::physical_devices::get_phyiscal_devices(
	const fngn_vk::instance& instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.vk_instance(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.vk_instance(), &deviceCount, devices.data());

	std::vector<physical_device> fngn_devices;

	for (auto& vkDevice : devices)
	{
		fngn_devices.push_back(physical_device(vkDevice));
	}

	return fngn_devices;
}

void fngn_vk::physical_devices::check_suitable_device_available(const fngn_vk::instance& instance)
{
	auto devices = get_phyiscal_devices(instance);

	if (!std::any_of(STD_RANGE(devices),
			[](const physical_device& device)
			{
				return device.is_suitable();
			}))
	{
		throw std::runtime_error("Could not find a suitable device for Vuklan!");
	}
}

void fngn_vk::physical_devices::print_availabe_devices(const fngn_vk::instance& instance)
{
	auto devices = get_phyiscal_devices(instance);

	for (auto& device : devices)
	{
		auto props = device.get_properties();

		std::cout << "Physical Device: " << std::endl
			<< "Device Name: " << props.deviceName << std::endl
			<< "Device Type: " << props.deviceType << std::endl
			<< "Driver Version: " << props.driverVersion << std::endl;
	}
}