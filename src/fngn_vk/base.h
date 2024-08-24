#pragma once
#define GLFW_INCLUDE_VULKAN
#ifdef _WINDOWS
#define GLFW_DLL
#endif
#include <GLFW/glfw3.h>

#define PREVENT_COPY(class_name) \
	class_name(const class_name&) = delete; \
	class_name(class_name&&) = delete; \
	class_name& operator = (const class_name&) = delete; \
	class_name& operator = (class_name&&) = delete;

#define STD_RANGE(vec) vec.begin(), vec.end()

namespace fngn_vk
{
	void fnvk_verify(VkResult result, const char* operation);
	const char* to_human_readable(VkResult result);
}