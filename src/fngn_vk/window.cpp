#include <fngn_vk/window.h>

fngn_vk::window::window()
{
}

void fngn_vk::window::run()
{
	initialize_window();
	initialize_vulkan();
	main_loop();
	destroy();
}

std::vector<std::string> fngn_vk::window::get_glfw_required_extensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	return std::vector<std::string>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

void fngn_vk::window::initialize_window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(800, 600, "Fungeon RTX", nullptr, nullptr);
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
