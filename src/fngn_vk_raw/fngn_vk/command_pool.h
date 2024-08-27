#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class logical_device;

	class command_pool
	{
	public:
		PREVENT_COPY(command_pool);

		command_pool(const logical_device& device);
		~command_pool();

		inline const VkCommandPool vk_handle() const { return m_pool; }
		inline const logical_device& device() const { return m_device; }

	private:
		const logical_device& m_device;
		VkCommandPool m_pool{};
	};
}