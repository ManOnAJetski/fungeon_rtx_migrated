#pragma once
#include <fngn_vk/base.h>
#include <vector>

namespace fngn_vk
{
	class window;

	class instance
	{
	public:
		PREVENT_COPY(instance);

		instance(const fngn_vk::window& window);
		~instance();

		inline VkInstance vk_handle() const { return m_instance; };

		const fngn_vk::window& window() const;
		const std::vector<const char*> get_enabled_extension_names() const;

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