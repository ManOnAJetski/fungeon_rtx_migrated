#pragma once
#include <fngn_vk/base.h>
#include <vector>

namespace fngn_vk
{
	class swap_chain;
	class pipeline_layout;
	class render_pass;
	class shader;

	class graphics_pipeline
	{
	public:
		PREVENT_COPY(graphics_pipeline);

		graphics_pipeline(
			const swap_chain& swap_chain,
			const pipeline_layout& layout,
			const render_pass& render_pass,
			const std::vector<const shader*> shaders);

		~graphics_pipeline();

		const VkPipeline vk_handle() const { return m_pipeline; }

	private:
		const swap_chain& m_swap_chain;
		const pipeline_layout& m_pipeline_layout;
		const render_pass& m_render_pass;
		VkPipeline m_pipeline{};
	};
}