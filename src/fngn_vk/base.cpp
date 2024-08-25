#include "base.h"
#include <vulkan/vk_enum_string_helper.h>
#include <stdexcept>
#include <utils/exception.h>

void fngn_vk::fnvk_verify(VkResult result, const char* operation)
{
	if (result != VK_SUCCESS)
	{
		fngn_throw(std::runtime_error(std::string("[") + operation + std::string("] Failed! - ") + to_human_readable(result)));
	}
}

const char* fngn_vk::to_human_readable(VkResult result)
{
	return string_VkResult(result);
}
