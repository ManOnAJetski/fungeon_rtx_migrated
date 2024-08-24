#include <fngn_vk/base.h>
#include <memory>

namespace fngn_vk
{
	class window final
	{
	public:
		window() = default;
		~window() = default;

		void run();

		PREVENT_COPY(window);

	private:
		void initialize_window();
		void initialize_vulkan();
		void main_loop();
		void destroy();

		GLFWwindow* m_window{};
	};
}