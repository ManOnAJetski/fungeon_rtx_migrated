#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/window.h>
#include <fngn_vk/validator.h>
#include <vector>
#include <string>

namespace fngn_vk
{
	class instance
	{
	public:
		PREVENT_COPY(instance);

		instance(const fngn_vk::window& window);
		~instance();

		inline VkInstance vk_instance() const { return m_instance; };
		inline const fngn_vk::window& window() const { return m_window; };

		inline const std::vector<const char*> get_enabled_extension_names() const { return m_window.get_glfw_required_extension_names(); };

	private:
		void create_instance();
		VkApplicationInfo construct_app_info();
		VkInstanceCreateInfo construct_instance_creation_info(VkApplicationInfo* app_info);

		void check_extensions(const VkInstanceCreateInfo& info);
		void setup_debug_messages();

		std::vector<VkExtensionProperties> get_available_extensions();

		VkInstance m_instance;
		const fngn_vk::window& m_window;
		VkDebugUtilsMessengerEXT m_debug_messenger{};

		std::vector<VkExtensionProperties> m_enabled_extenions;

	};
}