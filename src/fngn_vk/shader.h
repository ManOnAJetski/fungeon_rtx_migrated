#pragma once
#include <fngn_vk/base.h>
#include <string>
#include <vector>

namespace fngn_vk
{
	class logical_device;

	class shader
	{
	public:
		PREVENT_COPY(shader)

		shader(const logical_device& device, const std::string& filename);
		shader(const logical_device& device, const std::vector<char>& code);
		~shader();

		VkPipelineShaderStageCreateInfo create_shader_stage(VkShaderStageFlagBits stage) const;

	private:
		static const std::vector<char> read_shader_file(const std::string& filename);

		const logical_device& m_device;
		VkShaderModule m_shader_module;
	};
}