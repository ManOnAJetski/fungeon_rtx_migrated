#pragma once
#include <fngn_vk/base.h>
#include <fngn_vk/physical_device.h>
#include <vector>
#include <optional>
namespace fngn_vk
{
	class logical_device
	{
	public:
		explicit logical_device(const physical_device& physical_device);
		~logical_device();

		inline VkDevice vk_device() const { return m_device; }
		inline const physical_device& underlying_physical() const { return m_physical_device; }

	private:
		const physical_device& m_physical_device;
		VkDevice m_device = VK_NULL_HANDLE;
	};
}