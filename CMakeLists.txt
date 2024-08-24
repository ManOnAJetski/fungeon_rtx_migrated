cmake_minimum_required(version 3.20)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)

project(fungeon_rtx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

IF (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK could not be found! Please verify the Vulkan SDK has been installed.")
ELSE()
    message(STATUS ${Vulkan_LIBRARY})
ENDIF()

add_subdirectory(src)
