#include "binary_semaphore.h"
#include "logical_device.h"

fngn_vk::binary_semaphore::binary_semaphore(const logical_device& device)
	: m_device(device)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(m_device.vk_handle(), &semaphoreInfo, nullptr, &m_semaphore);
}

fngn_vk::binary_semaphore::~binary_semaphore()
{
	if (m_semaphore)
	{
		vkDestroySemaphore(m_device.vk_handle(), m_semaphore, nullptr);
		m_semaphore = nullptr;
	}
}
