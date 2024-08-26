#include "swap_chain.h"
#include "logical_device.h"
#include "physical_device.h"
#include "surface.h"
#include "image_view.h"
#include <limits>
#include <algorithm>
#include <ranges>

fngn_vk::swap_chain::swap_chain(
	const logical_device& logical_device,
	const fngn_vk::surface& surface)
	: m_logical_device(logical_device), m_surface(surface)
{
	m_swap_chain_support = m_logical_device.underlying_physical().query_swap_chain_support(m_surface);
	// Use one more than minumum to prevent driver stalls
	uint32_t image_count = m_swap_chain_support.capabilities.minImageCount + 1;

	if (m_swap_chain_support.capabilities.maxImageCount > 0 && image_count > m_swap_chain_support.capabilities.maxImageCount) {
		image_count = m_swap_chain_support.capabilities.maxImageCount;
	}

	m_surface_format = choose_surface_format();
	m_extents = choose_extent();
	m_present_mode = choose_present_mode();

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface.vk_handle();
	createInfo.minImageCount = image_count;
	createInfo.imageFormat = m_surface_format.format;
	createInfo.imageColorSpace = m_surface_format.colorSpace;
	createInfo.imageExtent = m_extents;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	auto indices = m_logical_device.underlying_physical().get_available_queue_families(surface);

	uint32_t queueFamilyIndices[] = { indices.graphics_family.value(), indices.present_family.value() };

	if (indices.graphics_family != indices.present_family) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = m_swap_chain_support.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = m_present_mode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	createInfo.pNext = VK_NULL_HANDLE;

	auto device = m_logical_device.vk_handle();

	fnvk_verify(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swap_chain), "Creating swap chain failed");

	m_min_image_count = max(2u, m_swap_chain_support.capabilities.minImageCount);

	vkGetSwapchainImagesKHR(m_logical_device.vk_handle(), m_swap_chain, &image_count, nullptr);
	m_images.resize(image_count);
	vkGetSwapchainImagesKHR(m_logical_device.vk_handle(), m_swap_chain, &image_count, m_images.data());

	m_image_views.reserve(image_count);
	for (const auto& image : m_images)
	{
		m_image_views.push_back(std::make_unique<image_view>(m_logical_device, image, m_surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT));
	}
}

fngn_vk::swap_chain::~swap_chain()
{
	if (m_swap_chain)
	{
		vkDestroySwapchainKHR(m_logical_device.vk_handle(), m_swap_chain, nullptr);
	}
}

const VkExtent2D fngn_vk::swap_chain::extents() const
{
	return m_extents;
}

const VkSurfaceFormatKHR fngn_vk::swap_chain::surface_format() const
{
	return m_surface_format;
}

const fngn_vk::logical_device& fngn_vk::swap_chain::device() const
{
	return m_logical_device;
}

const fngn_vk::surface& fngn_vk::swap_chain::surface() const
{
	return m_surface;
}

const std::vector<std::unique_ptr<fngn_vk::image_view>>& fngn_vk::swap_chain::image_views() const
{
	return m_image_views;
}

VkSurfaceFormatKHR fngn_vk::swap_chain::choose_surface_format() const
{
	for (const auto& available_format : m_swap_chain_support.formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return available_format;
		}
	}

	return m_swap_chain_support.formats.front();
}

VkPresentModeKHR fngn_vk::swap_chain::choose_present_mode() const
{
	for (const auto& available_present_mode : m_swap_chain_support.present_modes) {
		if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return available_present_mode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D fngn_vk::swap_chain::choose_extent() const
{
	if (m_swap_chain_support.capabilities.currentExtent.width != (std::numeric_limits<uint32_t>::max)())
	{
		return m_swap_chain_support.capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_surface.glfw_window(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(
			actualExtent.width,
			m_swap_chain_support.capabilities.minImageExtent.width,
			m_swap_chain_support.capabilities.maxImageExtent.width);

		actualExtent.height = std::clamp(
			actualExtent.height,
			m_swap_chain_support.capabilities.minImageExtent.height,
			m_swap_chain_support.capabilities.maxImageExtent.height);

		return actualExtent;
	}
}
