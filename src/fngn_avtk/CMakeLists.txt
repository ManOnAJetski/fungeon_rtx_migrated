set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib" )

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/bin" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/lib" )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/lib" )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(exe_name fngn_vk_avtk)
add_executable(${exe_name})

target_sources(${exe_name}
    PRIVATE
        main.cpp
        src/fngn_avtk_hello_triangle.cpp
        src/fngn_avtk_path_tracing.cpp
        include/fngn_avtk_hello_triangle.h
        include/fngn_avtk_path_tracing.h
		include/camera_path.hpp
		include/ui_helper.hpp
)

target_include_directories(fngn_vk_avtk PUBLIC include/)

set_target_properties(fngn_vk_avtk PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX} VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_include_directories(fngn_vk_avtk PUBLIC Auto_Vk_Toolkit)
target_link_libraries(fngn_vk_avtk PUBLIC Auto_Vk_Toolkit)

# Assets
include(${Auto_Vk_Toolkit_SOURCE_DIR}/cmake/post_build_helper/add_post_build_commands.cmake)

set(autoVkToolkitStarter_BINARY_DIR "${PROJECT_BINARY_DIR}/bin")

add_post_build_commands(fngn_vk_avtk
    ${PROJECT_SOURCE_DIR}/src/shaders
    ${autoVkToolkitStarter_BINARY_DIR}/shaders
    $<TARGET_FILE_DIR:fngn_vk_avtk>/assets
    ${PROJECT_SOURCE_DIR}/src/assets
    OFF)