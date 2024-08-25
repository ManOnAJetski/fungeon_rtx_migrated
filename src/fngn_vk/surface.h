#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/instance.h>

namespace fngn_vk
{
	class surface
	{
	public:
		PREVENT_COPY(surface);

		surface(const instance& instance);
		~surface();

		inline const VkSurfaceKHR& vk_handle() const { return m_surface; }
		inline GLFWwindow* glfw_window() const { return m_instance.window().glfw_window(); }

	private:
		const instance& m_instance;
		VkSurfaceKHR m_surface;
	};
}
