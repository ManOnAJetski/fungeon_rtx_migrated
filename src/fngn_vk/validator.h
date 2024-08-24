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

		explicit validator();

		inline constexpr bool is_enabled() const { return enableValidationLayers; }
		inline constexpr int get_num_validation_layers() const { return enabled_validation_layers.size(); }
		inline constexpr auto get_validation_layers() const { return enabled_validation_layers.data(); }

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		bool check_validation_layer_support();

	private:
		const std::vector<const char*> enabled_validation_layers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

	};
}