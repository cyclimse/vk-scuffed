add_subdirectory(utils)
target_include_directories(scuffedcraft PRIVATE utils)

target_sources(scuffedcraft
    PRIVATE
        vk_engine.cpp
        vk_utils.cpp
)