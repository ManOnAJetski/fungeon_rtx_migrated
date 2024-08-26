#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class physical_device;
	class surface;

	class logical_device
	{
	public:
		explicit logical_device(
			const physical_device& physical_device,
			const surface& surface);
		~logical_device();

		inline VkDevice vk_handle() const { return m_device; }
		inline VkQueue graphics_queue() const { return m_graphics_queue; }
		inline VkQueue present_queue() const { return m_present_queue; }
		inline const physical_device& underlying_physical() const { return m_physical_device; }
		inline const fngn_vk::surface& surface() const { return m_surface; }

	private:
		const physical_device& m_physical_device;
		const fngn_vk::surface& m_surface;
		VkDevice m_device = VK_NULL_HANDLE;
		VkQueue m_graphics_queue;
		VkQueue m_present_queue;
	};
}