set(exe_name ${MAIN_PROJECT})

set(src_files
	main.cpp
)

set(src_files_fngn_vk
	fngn_vk/base.h
	fngn_vk/base.cpp
	fngn_vk/physical_devices.h
	fngn_vk/physical_devices.cpp
	fngn_vk/logical_device.h
	fngn_vk/logical_device.cpp
	fngn_vk/physical_device.h
	fngn_vk/physical_device.cpp
	fngn_vk/device_queue.h
	fngn_vk/device_queue.cpp
	fngn_vk/instance.cpp
	fngn_vk/instance.h
	fngn_vk/surface.cpp
	fngn_vk/surface.h
	fngn_vk/window.cpp
	fngn_vk/window.h
	fngn_vk/validator.cpp
	fngn_vk/validator.h
)

source_group("fungeon_rtx_app" FILES ${src_files})
source_group("fngn_vk" FILES ${src_files_fngn_vk})

add_executable(
	${exe_name}
	${src_files}
	${src_files_fngn_vk} 
)

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_include_directories(${exe_name} PRIVATE . ${Vulkan_INCLUDE_DIRS} ${GLFW_INLUDE_DIRS})
target_link_directories(${exe_name} PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(${exe_name} PRIVATE ${Vulkan_LIBRARIES} glfw)
