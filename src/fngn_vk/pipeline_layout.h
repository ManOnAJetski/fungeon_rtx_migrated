#pragma once
#include <fngn_vk/base.h>

namespace fngn_vk
{
	struct pipeline_creation_info
	{
		VkPipelineDynamicStateCreateInfo dynamic_state{};
		VkPipelineVertexInputStateCreateInfo vertex_input{};
		VkPipelineInputAssemblyStateCreateInfo input_assembly{};
		VkPipelineViewportStateCreateInfo viewport_state{};
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		VkPipelineMultisampleStateCreateInfo multi_sampling{};
		VkPipelineColorBlendStateCreateInfo color_blending{};
	};

	class swap_chain;
	class pipeline_layout
	{
	public:
		PREVENT_COPY(pipeline_layout);

		pipeline_layout(const swap_chain& swap_chain);
		~pipeline_layout();

		inline const pipeline_creation_info* creation_info() const { return &m_creation_info; }
		inline const VkPipelineLayout vk_handle() const { return m_layout; }

	private:
		const swap_chain& m_swap_chain;
		VkPipelineLayout m_layout{};
		pipeline_creation_info m_creation_info;
	};
}