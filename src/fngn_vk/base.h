#pragma once
#define GLFW_INCLUDE_VULKAN
#ifdef _WINDOWS
#define GLFW_DLL
#endif
#include <GLFW/glfw3.h>

#define PREVENT_COPY(class_name) class_name(const class_name&) = delete;\
                                 class_name& operator=(const class_name&) = delete