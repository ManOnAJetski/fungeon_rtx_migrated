#include "shader.h"
#include "logical_device.h"
#include <fstream>

fngn_vk::shader::shader(const logical_device& device, const std::string& filename)
	: shader(device, read_shader_file(filename))
{
}

fngn_vk::shader::shader(const logical_device& device, const std::vector<char>& code)
	: m_device(device)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	fngn_vk::fnvk_verify(vkCreateShaderModule(device.vk_handle(), &createInfo, nullptr, &m_shader_module), "create shader module");
}

fngn_vk::shader::~shader()
{
	if (m_shader_module != nullptr)
	{
		vkDestroyShaderModule(m_device.vk_handle(), m_shader_module, nullptr);
		m_shader_module = nullptr;
	}
}

VkPipelineShaderStageCreateInfo fngn_vk::shader::create_shader_stage(VkShaderStageFlagBits stage) const
{
	VkPipelineShaderStageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.stage = stage;
	createInfo.module = m_shader_module;
	createInfo.pName = "main";

	return createInfo;
}

const std::vector<char> fngn_vk::shader::read_shader_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file '" + filename + "'");
	}

	const auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
