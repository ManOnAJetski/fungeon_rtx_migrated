#include "shader.h"
#include "logical_device.h"
#include <fstream>
#include <utils/exception.h>

fngn_vk::shader::shader(
	const logical_device& device,
	const std::string& filename,
	VkShaderStageFlagBits flag_bits)
	: shader(device, read_shader_file(filename), flag_bits)
{
}

fngn_vk::shader::shader(const logical_device& device, const std::filesystem::path& path, VkShaderStageFlagBits flag_bits)
	: shader(device, read_shader_file(path.string()), flag_bits)
{
}

fngn_vk::shader::shader(const logical_device& device, const std::vector<char>& code, VkShaderStageFlagBits flag_bits)
	: m_device(device), m_flag_bits(flag_bits)
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

VkPipelineShaderStageCreateInfo fngn_vk::shader::create_shader_stage() const
{
	VkPipelineShaderStageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.stage = m_flag_bits;
	createInfo.module = m_shader_module;
	createInfo.pName = "main";

	return createInfo;
}

const std::vector<char> fngn_vk::shader::read_shader_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		fngn_throw(std::runtime_error("failed to open file '" + filename + "'"));
	}

	const auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
