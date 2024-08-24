#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/window.h>
#include <vector>
#include <string>

namespace fngn_vk
{
	class instance
	{
	public:
		PREVENT_COPY(instance);

		instance(const fngn_vk::window& window);

	private:
		void create_instance();
		VkApplicationInfo construct_app_info();
		VkInstanceCreateInfo construct_instance_creation_info(VkApplicationInfo* app_info);

		void check_extensions(const VkInstanceCreateInfo& info);

		std::vector<VkExtensionProperties> get_available_extensions();


		VkInstance m_instance;
		const fngn_vk::window& m_window;
	};
}