#pragma once
#include <fngn_vk/base.h>
#include <string>
#include <vector>
#include <filesystem>

namespace fngn_vk
{
	class logical_device;

	class shader
	{
	public:
		PREVENT_COPY(shader)

		shader(const logical_device& device, const std::string& filename, VkShaderStageFlagBits flag_bits);
		shader(const logical_device& device, const std::filesystem::path& path, VkShaderStageFlagBits flag_bits);
		shader(const logical_device& device, const std::vector<char>& code, VkShaderStageFlagBits flag_bits);
		~shader();

		VkPipelineShaderStageCreateInfo create_shader_stage() const;

	private:
		static const std::vector<char> read_shader_file(const std::string& filename);
		VkShaderStageFlagBits m_flag_bits;
		const logical_device& m_device;
		VkShaderModule m_shader_module;
	};
}