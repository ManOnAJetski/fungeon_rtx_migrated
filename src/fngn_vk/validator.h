#pragma once
#include <fngn_vk/base.h>
#include <vector>
#include <iostream>

namespace fngn_vk
{
	class validator
	{
	public:
		PREVENT_COPY(validator);

		inline constexpr static bool is_enabled() { return enableValidationLayers; }
		inline constexpr static int get_num_validation_layers() { return enabled_validation_layers.size(); }
		inline constexpr static int get_num_validation_extensions() { return enabled_validation_extensions.size(); }
		inline constexpr static auto get_validation_layers() { return enabled_validation_layers.data(); }
		inline constexpr static auto get_validation_extensions() { return enabled_validation_extensions.data(); }

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		static VkResult create_debug_utils_messenger_ext(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);

		static void destroy_debug_utils_messenger_ext(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);

		static void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		static bool check_validation_layer_support();

		const static std::vector<const char*> enabled_validation_layers;
		const static std::vector<const char*> enabled_validation_extensions;
	private:

#ifdef NDEBUG
		static const bool enableValidationLayers = false;
#else
		static const bool enableValidationLayers = true;
#endif

	};
}
