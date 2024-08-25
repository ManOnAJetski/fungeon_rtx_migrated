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

		inline const VkPhysicalDeviceProperties& get_properties() const { return m_props; };
		inline const VkPhysicalDeviceFeatures& get_features() const { return m_features; };
		inline const std::vector<const char*>& get_enabled_extension_names() const { return m_enabled_extension_names; };
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

		const void print_device_info() const;
	private:

		const std::vector<const char*> m_required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		const std::vector<VkExtensionProperties> get_available_extensions() const;
		const bool check_device_extension_support() const;

		VkPhysicalDevice m_device = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties m_props;
		VkPhysicalDeviceFeatures m_features;
		std::vector<const char*> m_enabled_extension_names;
	};
}