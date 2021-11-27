cmake_minimum_required(VERSION 3.16...3.21)

# cmake-lint: disable=C0103
set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "")

include(FindVulkan)
include(FindBoost)

project(scuffedcraft LANGUAGES CXX)

add_executable(scuffedcraft src/main.cpp)

add_subdirectory(src)
target_include_directories(scuffedcraft PRIVATE src)

set_property(TARGET scuffedcraft PROPERTY CXX_STANDARD 20)

target_link_options(scuffedcraft PRIVATE $<$<CONFIG:Asan>:-fsanitize=address
                    -fno-omit-frame-pointer>)

target_compile_options(
  scuffedcraft
  PRIVATE $<$<CONFIG:Debug>:-Wall
          -Wextra
          -Wpedantic
          -Og>
          $<$<CONFIG:Asan>:-Wall
          -Wextra
          -Wpedantic
          -Og
          -fsanitize=address
          -fno-omit-frame-pointer>
          $<$<CONFIG:Release>:-O3>)

add_custom_command(
  TARGET scuffedcraft
  PRE_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE_DIR:scuffedcraft>/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/asan.supp
          $<TARGET_FILE_DIR:scuffedcraft>/asan.supp
  COMMENT "Copying compile_commands.json and asan.supp ..."
  VERBATIM)

find_package(Vulkan REQUIRED)
find_package(
  Boost 1.69.0
  COMPONENTS json
  REQUIRED)
target_compile_definitions(scuffedcraft
                           PUBLIC -DVULKAN_HPP_NO_STRUCT_CONSTRUCTORS)
target_compile_definitions(scuffedcraft PUBLIC -DVULKAN_HPP_NO_CONSTRUCTORS)

# find_package(glm REQUIRED)

find_package(glfw3 REQUIRED)
target_compile_definitions(scuffedcraft PUBLIC -DGLFW_INCLUDE_NONE)
target_compile_definitions(scuffedcraft PUBLIC -DGLFW_INCLUDE_VULKAN)

# target_include_directories(game PRIVATE "libs/stb_image")

target_include_directories(scuffedcraft PRIVATE "libs")

# Precompiling headers achieves two things here: -- faster compilation times --
# avoiding warning pollution from VMA
target_precompile_headers(scuffedcraft PRIVATE [["vk_mem_alloc.h"]])

target_link_libraries(scuffedcraft PRIVATE Vulkan::Vulkan Boost::boost glfw)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# Shader compilation stage
find_program(
  GLSL_VALIDATOR glslangValidator
  HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
file(
  GLOB_RECURSE GLSL_SOURCE_FILES "${PROJECT_SOURCE_DIR}/assets/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/assets/shaders/*.vert"
  "${PROJECT_SOURCE_DIR}/assets/shaders/*.comp")

# cmake-lint: disable=C0103
foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/assets/shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL}
    COMMENT "Running glslValidator ...")
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
  Shaders
  DEPENDS ${SPIRV_BINARY_FILES}
  COMMENT "Building shaders ...")

# Assets copy
add_custom_target(
  copy_assets
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-resources.cmake
  COMMENT "Copying assets ...")
add_dependencies(scuffedcraft copy_assets)
