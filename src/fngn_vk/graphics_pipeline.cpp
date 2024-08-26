#include "graphics_pipeline.h"
#include "swap_chain.h"
#include "logical_device.h"
#include "pipeline_layout.h"
#include "render_pass.h"
#include "shader.h"
#include <iterator>
#include <algorithm>

fngn_vk::graphics_pipeline::graphics_pipeline(
	const swap_chain& swap_chain,
	const pipeline_layout& layout,
	const render_pass& render_pass,
	const std::vector<const shader*> shaders)
	: m_swap_chain(swap_chain),
	m_pipeline_layout(layout),
	m_render_pass(render_pass)
{
	VkGraphicsPipelineCreateInfo pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;

	std::vector<VkPipelineShaderStageCreateInfo> shader_stage_info;

	std::transform(
		STD_RANGE(shaders),
		std::back_inserter(shader_stage_info),
		[](const shader* shader) { return shader->create_shader_stage(); });

	auto pipeline_creation_info = layout.creation_info();

	pipeline_info.pStages = shader_stage_info.data();
	pipeline_info.pVertexInputState = &pipeline_creation_info->vertex_input;
	pipeline_info.pInputAssemblyState = &pipeline_creation_info->input_assembly;
	pipeline_info.pViewportState = &pipeline_creation_info->viewport_state;
	pipeline_info.pRasterizationState = &pipeline_creation_info->rasterizer;
	pipeline_info.pMultisampleState = &pipeline_creation_info->multi_sampling;
	pipeline_info.pDepthStencilState = nullptr; // Optional
	pipeline_info.pColorBlendState = &pipeline_creation_info->color_blending;
	pipeline_info.pDynamicState = &pipeline_creation_info->dynamic_state;
	pipeline_info.layout = m_pipeline_layout.vk_handle();
	pipeline_info.renderPass = m_render_pass.vk_handle();
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipeline_info.basePipelineIndex = -1; // Optional

	fngn_vk::fnvk_verify(
		vkCreateGraphicsPipelines(m_swap_chain.device().vk_handle(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline),
		"Creating graphics pipeline");
}

fngn_vk::graphics_pipeline::~graphics_pipeline()
{
	if (m_pipeline)
	{
		vkDestroyPipeline(m_swap_chain.device().vk_handle(), m_pipeline, nullptr);
		m_pipeline = nullptr;
	}
}
