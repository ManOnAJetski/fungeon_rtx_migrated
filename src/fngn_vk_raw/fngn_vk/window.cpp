#include "window.h"
#include "renderer.h"

fngn_vk::window::window()
{
	initialize_window();
}

void fngn_vk::window::run()
{
	main_loop();
	destroy();
}

const std::vector<const char*> fngn_vk::window::get_glfw_required_extension_names() const
{
	std::vector<const char*> res;
	for (auto& ext : m_required_extensions)
		res.push_back(ext.data());
	return res;
}

void fngn_vk::window::attach(std::unique_ptr<renderer> renderer)
{
	m_renderer = std::move(renderer);
}

void fngn_vk::window::initialize_window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(800, 600, "Fungeon RTX", nullptr, nullptr);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	m_required_extensions = std::vector<std::string>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

void fngn_vk::window::main_loop()
{
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		m_renderer->draw();
	}

	m_renderer->wait_for_device();
}

void fngn_vk::window::destroy()
{
	m_renderer.reset();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
