#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/logical_device.h>
#include <fngn_vk/surface.h>

namespace fngn_vk
{
	class swap_chain
	{
	public:
		PREVENT_COPY(swap_chain);

		explicit swap_chain(
			const logical_device& logical_device,
			const surface& surface);

	private:
		VkSurfaceFormatKHR choose_surface_format() const;
		VkPresentModeKHR choose_present_mode() const;
		VkExtent2D choose_extent() const;

		VkSurfaceFormatKHR m_surface_format;
		VkPresentModeKHR m_present_mode;
		VkExtent2D m_extents;
		std::vector<VkImage> m_images;
		const surface& m_surface;
		const logical_device& m_logical_device;
		physical_device::swap_chain_details m_swap_chain_support;
		VkSwapchainKHR m_swap_chain{};
	};
}