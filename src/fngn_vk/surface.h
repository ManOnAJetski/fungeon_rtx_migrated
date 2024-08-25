#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class instance;

	class surface
	{
	public:
		PREVENT_COPY(surface);

		surface(const instance& instance);
		~surface();

		inline const VkSurfaceKHR& vk_handle() const { return m_surface; }
		GLFWwindow* glfw_window() const;

	private:
		const instance& m_instance;
		VkSurfaceKHR m_surface;
	};
}
