set(exe_name ${MAIN_PROJECT})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/bin" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/lib" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/lib" )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(src_files
	main.cpp
)

set(src_files_fngn_vk
	fngn_vk/base.h
	fngn_vk/base.cpp
	fngn_vk/command_pool.h
	fngn_vk/command_pool.cpp
	fngn_vk/command_buffer.h
	fngn_vk/command_buffer.cpp
	fngn_vk/physical_devices.h
	fngn_vk/physical_devices.cpp
	fngn_vk/renderer.h
	fngn_vk/renderer.cpp
	fngn_vk/logical_device.h
	fngn_vk/logical_device.cpp
	fngn_vk/physical_device.h
	fngn_vk/physical_device.cpp
	fngn_vk/device_queue.h
	fngn_vk/device_queue.cpp
	fngn_vk/graphics_pipeline.h
	fngn_vk/graphics_pipeline.cpp
	fngn_vk/pipeline_layout.h
	fngn_vk/pipeline_layout.cpp
	fngn_vk/instance.cpp
	fngn_vk/instance.h
	fngn_vk/image.cpp
	fngn_vk/image.h
	fngn_vk/image_view.cpp
	fngn_vk/image_view.h
	fngn_vk/render_pass.cpp
	fngn_vk/render_pass.h
	fngn_vk/shader.cpp
	fngn_vk/shader.h
	fngn_vk/surface.cpp
	fngn_vk/surface.h
	fngn_vk/window.cpp
	fngn_vk/window.h
	fngn_vk/swap_chain.cpp
	fngn_vk/swap_chain.h
	fngn_vk/validator.cpp
	fngn_vk/validator.h
	fngn_vk/frame_buffer.cpp
	fngn_vk/frame_buffer.h
	fngn_vk/binary_semaphore.cpp
	fngn_vk/binary_semaphore.h
	fngn_vk/fence.cpp
	fngn_vk/fence.h
)

set(src_files_utils
	utils/exception.h
)

source_group("fungeon_rtx_app" FILES ${src_files})
source_group("fngn_vk" FILES ${src_files_fngn_vk})
source_group("utils" FILES ${src_files_utils})

add_executable(
	${exe_name}
	${src_files}
	${src_files_fngn_vk}
	${src_files_utils}
)

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_include_directories(${exe_name} PRIVATE . ${Vulkan_INCLUDE_DIRS} ${GLFW_INLUDE_DIRS})
target_link_directories(${exe_name} PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(${exe_name} PRIVATE ${Vulkan_LIBRARIES} glfw Boost::boost Boost::exception)

find_program(GLSLC glslc)
set(SHADERS_IN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADERS_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
file(GLOB SHADERS "${SHADERS_IN_DIR}/*.vert" "${SHADERS_IN_DIR}/*.frag")
file(MAKE_DIRECTORY ${SHADERS_OUT_DIR})

add_custom_command(
  TARGET ${exe_name}
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_OUT_DIR}
  COMMENT "Creating ${SHADERS_OUT_DIR}"
)

foreach(SHADER_FILE ${SHADERS}) 
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(SHADER_OUT_NAME "${SHADERS_OUT_DIR}/${SHADER_NAME}.spv")

	add_custom_command(		
		TARGET ${exe_name}
		PRE_BUILD
		MAIN_DEPENDENCY ${SHADER_FILE}
		COMMAND ${GLSLC} ${SHADER_FILE} "-o" ${SHADER_OUT_NAME}
		COMMENT "Compiling ${SHADER_OUT_NAME}")
		
	list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach()

add_custom_target(shaders ALL)
add_dependencies(${MAIN_PROJECT} shaders)
