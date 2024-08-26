#pragma once
#include <fngn_vk/base.h>
#include <memory>
#include <vector>
#include <string>

namespace fngn_vk
{
	class renderer;

	class window final
	{
	public:
		PREVENT_COPY(window);
		window();

		void run();

		inline GLFWwindow* glfw_window() const { return m_window; }
		const std::vector<const char*> get_glfw_required_extension_names() const;

		void attach(std::unique_ptr<renderer> renderer);

	private:
		void initialize_window();
		void main_loop();
		void destroy();

		std::vector<std::string> get_glfw_required_extensions() const;

		GLFWwindow* m_window{};
		std::unique_ptr<renderer> m_renderer;
		std::vector<std::string> m_required_extensions;
	};
}