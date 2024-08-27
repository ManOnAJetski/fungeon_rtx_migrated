#pragma once
#include <fngn_vk/base.h>
#include <vector>

namespace fngn_vk
{
	class physical_device;
	class instance;
	class surface;

	class physical_devices
	{
	public:
		PREVENT_COPY(physical_devices);

		static const std::vector<fngn_vk::physical_device> get_phyiscal_devices(const fngn_vk::instance& instance);
		static void check_suitable_device_available(
			const fngn_vk::instance& instance,
			const fngn_vk::surface& surface);
		static void print_devices(const fngn_vk::instance& instance, const std::vector<fngn_vk::physical_device>& devices);
	};
}
