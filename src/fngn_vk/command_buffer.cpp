#include "command_buffer.h"
#include "command_pool.h"
#include "logical_device.h"

fngn_vk::command_buffer::command_buffer(const command_pool& pool)
{
	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = pool.vk_handle();
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = 1;

	fngn_vk::fnvk_verify(vkAllocateCommandBuffers(pool.device().vk_handle(), &alloc_info, &m_buffer), "Creating command buffer");
}

void fngn_vk::command_buffer::record() const
{
	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = 0; // Optional
	begin_info.pInheritanceInfo = nullptr; // Optional

	fngn_vk::fnvk_verify(vkBeginCommandBuffer(m_buffer, &begin_info), "Beginning command buffer");
}

void fngn_vk::command_buffer::end() const
{
	fngn_vk::fnvk_verify(vkEndCommandBuffer(m_buffer), "Ending command buffer");
}
