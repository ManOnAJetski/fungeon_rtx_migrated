#include "physical_devices.h"
#include "physical_device.h"
#include "surface.h"
#include "instance.h"
#include <algorithm>
#include <stdexcept>
#include <type_traits>

const std::vector<fngn_vk::physical_device> fngn_vk::physical_devices::get_phyiscal_devices(
	const fngn_vk::instance& instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance.vk_handle(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance.vk_handle(), &deviceCount, devices.data());

	std::vector<physical_device> fngn_devices;

	for (auto& vkDevice : devices)
	{
		fngn_devices.emplace_back(physical_device(vkDevice, instance));
	}

	return fngn_devices;
}

void fngn_vk::physical_devices::check_suitable_device_available(
	const fngn_vk::instance& instance,
	const fngn_vk::surface& surface)
{
	auto devices = get_phyiscal_devices(instance);

	if (!std::any_of(STD_RANGE(devices),
			[&](const physical_device& device)
			{
				return device.is_suitable_for(surface);
			}))
	{
		throw std::runtime_error("Could not find a suitable device for Vuklan!");
	}
}

void fngn_vk::physical_devices::print_devices(
	const fngn_vk::instance& instance,
	const std::vector<fngn_vk::physical_device>& devices)
{
	for (auto& device : devices)
	{
		device.print_device_info();
	}
}
