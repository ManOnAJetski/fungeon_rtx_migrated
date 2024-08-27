cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_DEBUG_POSTFIX _d)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

project(fungeon_rtx)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # <ranges> support requires /std:c++latest on MSVC
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif (MSVC)

find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED COMPONENTS exception)
include(dependencies/auto_vk_toolkit.cmake)
find_package(glfw3 REQUIRED)

IF (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK could not be found! Please verify the Vulkan SDK has been installed.")
ELSE()
    message(STATUS ${Vulkan_LIBRARY})
ENDIF()

set(RAW_MAIN_PROJECT "fungeon_rtx_raw_app")

add_subdirectory(src)
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${RAW_MAIN_PROJECT})
