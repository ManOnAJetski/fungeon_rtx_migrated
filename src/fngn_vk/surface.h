#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/instance.h>

namespace fngn_vk
{
	class surface
	{
	public:
		PREVENT_COPY(surface);

		surface(
			const instance& instance,
			GLFWwindow* window);
		~surface();

		inline const VkSurfaceKHR& vk_surface() const { return m_surface; }

	private:
		const instance& m_instance;
		VkSurfaceKHR m_surface;
	};
}
