#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class swap_chain;
	class command_buffer;
	class frame_buffer;
	class graphics_pipeline;

	class render_pass
	{
	public:
		PREVENT_COPY(render_pass);

		render_pass(const swap_chain& swap_chain);
		~render_pass();

		inline VkRenderPass vk_handle() const { return m_pass; }
		const swap_chain& get_swap_chain() const;

	private:
		const swap_chain& m_swap_chain;
		VkRenderPass m_pass{};
	};
}