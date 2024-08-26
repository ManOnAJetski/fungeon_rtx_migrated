#include "fence.h"
#include "logical_device.h"

fngn_vk::fence::fence(const logical_device& device, VkFenceCreateFlags flags)
	: m_device(device)
{
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;
	vkCreateFence(m_device.vk_handle(), &fenceInfo, nullptr, &m_fence);
}

fngn_vk::fence::~fence()
{
	if (m_fence)
	{
		vkDestroyFence(m_device.vk_handle(), m_fence, nullptr);
		m_fence = nullptr;
	}
}
