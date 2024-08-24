#include "window.h"

void fngn_vk::window::run()
{
	initialize_window();
	initialize_vulkan();
	main_loop();
	destroy();
}

void fngn_vk::window::initialize_window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
}

void fngn_vk::window::initialize_vulkan()
{
}

void fngn_vk::window::main_loop()
{
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
	}
}

void fngn_vk::window::destroy()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
