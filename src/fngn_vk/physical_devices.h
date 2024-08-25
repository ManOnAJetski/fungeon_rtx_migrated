#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/physical_device.h>
#include <fngn_vk/instance.h>
#include <vector>

namespace fngn_vk
{
	class physical_devices
	{
	public:
		PREVENT_COPY(physical_devices);

		static const std::vector<fngn_vk::physical_device> get_phyiscal_devices(const fngn_vk::instance& instance);
		static void check_suitable_device_available(
			const fngn_vk::instance& instance,
			const fngn_vk::surface& surface);
		static void print_availabe_devices(const fngn_vk::instance& instance);
	};
}
