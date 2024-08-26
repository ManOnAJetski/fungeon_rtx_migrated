#pragma once
#include <fngn_vk/base.h>
#include <vector>
#include "physical_device.h"
#include <memory>

namespace fngn_vk
{
	class logical_device;
	class surface;
	class image_view;
	class frame_buffer;

	class swap_chain
	{
	public:
		PREVENT_COPY(swap_chain);

		explicit swap_chain(
			const logical_device& logical_device,
			const surface& surface);

		~swap_chain();

		const VkExtent2D extents() const;
		const VkSurfaceFormatKHR surface_format() const;
		const logical_device& device() const;
		const surface& surface() const;
		const std::vector<std::unique_ptr<image_view>>& image_views() const;

	private:
		VkSurfaceFormatKHR choose_surface_format() const;
		VkPresentModeKHR choose_present_mode() const;
		VkExtent2D choose_extent() const;

		VkSurfaceFormatKHR m_surface_format;
		VkPresentModeKHR m_present_mode;
		VkExtent2D m_extents;

		const fngn_vk::surface& m_surface;
		const logical_device& m_logical_device;
		physical_device::swap_chain_details m_swap_chain_support;

		uint32_t m_min_image_count;

		VkSwapchainKHR m_swap_chain{};
		std::vector<VkImage> m_images;
		std::vector<std::unique_ptr<image_view>> m_image_views;
	};
}