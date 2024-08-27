#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class command_pool;

	class command_buffer
	{
	public:
		PREVENT_COPY(command_buffer);

		command_buffer(const command_pool& pool);

		inline const VkCommandBuffer vk_handle() const { return m_buffer; }

		void record() const;
		void end() const;

	private:
		VkCommandBuffer m_buffer{};
	};
}