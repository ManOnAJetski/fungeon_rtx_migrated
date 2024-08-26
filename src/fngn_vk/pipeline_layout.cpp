#include "pipeline_layout.h"
#include "swap_chain.h"
#include "logical_device.h"


fngn_vk::pipeline_layout::pipeline_layout(const swap_chain& swap_chain): m_swap_chain(swap_chain)
{
	m_creation_info = pipeline_creation_info();
	memset(&m_creation_info, 0, sizeof(pipeline_creation_info));

	// Allow for scissoring or viewports in the pipeline
	std::vector<VkDynamicState> dynamic_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	m_creation_info.dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	m_creation_info.dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
	m_creation_info.dynamic_state.pDynamicStates = dynamic_states.data();

	// Hard coded for now
	m_creation_info.vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_creation_info.vertex_input.vertexBindingDescriptionCount = 0;
	m_creation_info.vertex_input.pVertexBindingDescriptions = nullptr; // Optional
	m_creation_info.vertex_input.vertexAttributeDescriptionCount = 0;
	m_creation_info.vertex_input.pVertexAttributeDescriptions = nullptr; // Optional

	// Only triangle vertices for now
	m_creation_info.input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_creation_info.input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	m_creation_info.input_assembly.primitiveRestartEnable = VK_FALSE;

	// Setup default viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swap_chain.extents().width;
	viewport.height = (float)m_swap_chain.extents().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = m_swap_chain.extents();

	// Specifiy a single viewport and scissor rect for now
	m_creation_info.viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_creation_info.viewport_state.viewportCount = 1;
	m_creation_info.viewport_state.pViewports = &viewport;
	m_creation_info.viewport_state.scissorCount = 1;
	m_creation_info.viewport_state.pScissors = &scissor;

	// Specifiy the rasteriser as follows:
	// - enable depth clamping to allow fragmant shaders beyond the
	//   near and far planes to be clamped back and drawn
	// - disable rasterizer discard so we actually render stuff
	// - set the rasteriser to fill
	// - enable back face culling, and set front face vertex order to clockwise
	m_creation_info.rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_creation_info.rasterizer.depthClampEnable = VK_FALSE;
	m_creation_info.rasterizer.rasterizerDiscardEnable = VK_FALSE;
	m_creation_info.rasterizer.lineWidth = 1.0f;
	m_creation_info.rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	m_creation_info.rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	m_creation_info.rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	m_creation_info.rasterizer.depthBiasEnable = VK_FALSE;
	m_creation_info.rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	m_creation_info.rasterizer.depthBiasClamp = 0.0f; // Optional
	m_creation_info.rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	// Disable MSAA for now
	m_creation_info.multi_sampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_creation_info.multi_sampling.sampleShadingEnable = VK_FALSE;
	m_creation_info.multi_sampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	m_creation_info.multi_sampling.minSampleShading = 1.0f; // Optional
	m_creation_info.multi_sampling.pSampleMask = nullptr; // Optional
	m_creation_info.multi_sampling.alphaToCoverageEnable = VK_FALSE; // Optional
	m_creation_info.multi_sampling.alphaToOneEnable = VK_FALSE; // Optional

	// set up alpha blending for the framebuffer
	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_TRUE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	m_creation_info.color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_creation_info.color_blending.logicOpEnable = VK_TRUE;
	m_creation_info.color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	m_creation_info.color_blending.attachmentCount = 1;
	m_creation_info.color_blending.pAttachments = &color_blend_attachment;
	m_creation_info.color_blending.blendConstants[0] = 0.0f; // Optional
	m_creation_info.color_blending.blendConstants[1] = 0.0f; // Optional
	m_creation_info.color_blending.blendConstants[2] = 0.0f; // Optional
	m_creation_info.color_blending.blendConstants[3] = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipeline_layout_info;
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 0; // Optional
	pipeline_layout_info.pSetLayouts = nullptr; // Optional
	pipeline_layout_info.pushConstantRangeCount = 0; // Optional
	pipeline_layout_info.pPushConstantRanges = nullptr; // Optional
	pipeline_layout_info.pNext = nullptr;

	fngn_vk::fnvk_verify(
		vkCreatePipelineLayout(
			m_swap_chain.device().vk_handle(),
			&pipeline_layout_info,
			nullptr,
			&m_layout),
		"Create pipeline layout");
}

fngn_vk::pipeline_layout::~pipeline_layout()
{
	if (m_layout)
	{
		vkDestroyPipelineLayout(m_swap_chain.device().vk_handle(), m_layout, nullptr);
		m_layout = nullptr;
	}
}
