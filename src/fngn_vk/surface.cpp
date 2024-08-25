#include <fngn_vk/surface.h>

fngn_vk::surface::surface(const instance& instance)
	: m_instance(instance)
{
	fnvk_verify(
		glfwCreateWindowSurface(m_instance.vk_instance(), glfw_window(), nullptr, &m_surface),
		"Creating surface failed!");
}

fngn_vk::surface::~surface()
{
	if (m_surface)
	{
		vkDestroySurfaceKHR(m_instance.vk_instance(), m_surface, nullptr);
		m_surface = nullptr;
	}
}
