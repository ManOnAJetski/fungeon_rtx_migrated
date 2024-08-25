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
		inline VkPhysicalDeviceFeatures get_features() const { return m_device_features; };
		inline VkPhysicalDevice vk_physical_device() const { return m_device; }

		struct queue_family_indicies
		{
			std::optional<uint32_t> graphics_family;
		};

		const queue_family_indicies get_available_queue_families() const;
	private:

		VkPhysicalDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_device_props;
		VkPhysicalDeviceFeatures m_device_features;
	};
}