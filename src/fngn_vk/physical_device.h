#pragma once
#include <fngn_vk/base.h>
#include <vector>
#include <optional>
namespace fngn_vk
{
	class physical_device
	{
	public:
		explicit physical_device(const VkPhysicalDevice& device);

		bool is_suitable() const;

		inline VkPhysicalDeviceProperties get_properties() const { return m_device_props; };
	private:
		struct queue_family_indicies
		{
			std::optional<uint32_t> graphics_family;
		};

		const queue_family_indicies get_available_queue_families() const;

		VkPhysicalDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_device_props;
	};
}