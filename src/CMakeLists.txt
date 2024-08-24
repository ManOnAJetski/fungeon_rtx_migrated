set(exe_name ${MAIN_PROJECT})

set(src_files
	main.cpp
)

source_group("fungeon_rtx_app" FILES ${src_files})

add_executable(
	${exe_name}
	${src_files}
)

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_include_directories(${exe_name} PRIVATE . ${Vulkan_INCLUDE_DIRS} ${GLFW_INLUDE_DIRS})
target_link_directories(${exe_name} PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(${exe_name} PRIVATE ${Vulkan_LIBRARIES} glfw)
