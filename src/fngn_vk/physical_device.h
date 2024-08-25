#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/surface.h>
#include <vector>
#include <optional>
namespace fngn_vk
{
	class physical_device
	{
	public:
		explicit physical_device(const VkPhysicalDevice& device);

		bool is_suitable_for(const surface& surface) const;

		inline const VkPhysicalDeviceProperties& get_properties() const { return m_device_props; };
		inline const VkPhysicalDeviceFeatures& get_features() const { return m_device_features; };
		inline const VkPhysicalDevice& vk_physical_device() const { return m_device; }

		struct queue_family_indicies
		{
			std::optional<uint32_t> graphics_family;
			std::optional<uint32_t> present_family;

			bool is_complete() const {
				return graphics_family.has_value() && present_family.has_value();
			}
		};

		const queue_family_indicies get_available_queue_families(const surface& surface) const;
	private:

		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VkPhysicalDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_device_props;
		VkPhysicalDeviceFeatures m_device_features;
	};
}