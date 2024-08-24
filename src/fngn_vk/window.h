#pragma once
#include <fngn_vk/base.h>
#include <memory>
#include <vector>
#include <string>

namespace fngn_vk
{
	class window final
	{
	public:
		PREVENT_COPY(window);
		window();

		void run();
		std::vector<std::string> get_glfw_required_extensions() const;

	private:
		void initialize_window();
		void initialize_vulkan();
		void main_loop();
		void destroy();

		GLFWwindow* m_window{};
	};
}