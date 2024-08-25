#include "surface.h"
#include "instance.h"
#include "window.h"

fngn_vk::surface::surface(const instance& instance)
	: m_instance(instance)
{
	fnvk_verify(
		glfwCreateWindowSurface(m_instance.vk_handle(), glfw_window(), nullptr, &m_surface),
		"Creating surface failed!");
}

fngn_vk::surface::~surface()
{
	if (m_surface)
	{
		vkDestroySurfaceKHR(m_instance.vk_handle(), m_surface, nullptr);
		m_surface = nullptr;
	}
}

GLFWwindow* fngn_vk::surface::glfw_window() const
{
	return m_instance.window().glfw_window();
}
