cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include(FindVulkan)

project(scuffedcraft LANGUAGES CXX)

add_executable(scuffedcraft
    src/main.cpp
)

add_subdirectory(src)

set_property(TARGET scuffedcraft PROPERTY CXX_STANDARD 20)

# target_link_options(scuffedcraft PRIVATE
#     $<$<CONFIG:Debug>:-fsanitize=address>
# )

target_compile_options(scuffedcraft PRIVATE
    $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -Og>
    $<$<CONFIG:Release>:-O3>
)

add_custom_command(
        TARGET scuffedcraft PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE_DIR:scuffedcraft>/compile_commands.json  ${CMAKE_SOURCE_DIR}/compile_commands.json
        COMMENT "Copying compile_commands.json" VERBATIM
)

find_package(Vulkan REQUIRED)
target_compile_definitions(scuffedcraft PUBLIC -DVULKAN_HPP_NO_STRUCT_CONSTRUCTORS)
target_compile_definitions(scuffedcraft PUBLIC -DVULKAN_HPP_NO_CONSTRUCTORS)

# find_package(glm REQUIRED)

find_package(glfw3 REQUIRED)
target_compile_definitions(scuffedcraft PUBLIC -DGLFW_INCLUDE_NONE)


#target_include_directories(game PRIVATE "libs/stb_image")
target_include_directories(scuffedcraft PRIVATE "libs")

# Precompiling headers achieves two things here:
#   -- faster compilation times
#   -- avoiding warning pollution from VMA
target_precompile_headers(scuffedcraft PRIVATE [["vk_mem_alloc.h"]])
# target_precompile_headers(scuffedcraft PRIVATE <vulkan/vulkan.hpp>)

#find_package(SPIRV-Tools REQUIRED)
#set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
#set(glslang_SOURCE_DIR "/usr/include/glslang")
#add_subdirectory(libs/shaderc)
#target_include_directories(shaderc PRIVATE ${SPIRV-Tools_INCLUDE_DIRS})

# target_link_libraries(game PRIVATE Vulkan::Vulkan shaderc glm glfw)
target_link_libraries(scuffedcraft PRIVATE Vulkan::Vulkan glfw)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()