#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	class swap_chain;

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