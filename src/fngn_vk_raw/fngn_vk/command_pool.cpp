#include "command_pool.h"
#include "logical_device.h"
#include "physical_device.h"
#include "surface.h"

fngn_vk::command_pool::command_pool(const logical_device& device)
	: m_device(device)
{
	VkCommandPoolCreateInfo pool_info{};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = m_device.underlying_physical().get_available_queue_families(device.surface()).graphics_family.value();

	fngn_vk::fnvk_verify(vkCreateCommandPool(m_device.vk_handle(), &pool_info, nullptr, &m_pool), "Creating command pool");
}

fngn_vk::command_pool::~command_pool()
{
	if (m_pool)
	{
		vkDestroyCommandPool(m_device.vk_handle(), m_pool, nullptr);
		m_pool = nullptr;
	}
}
