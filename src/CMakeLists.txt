target_sources(scuffedcraft PRIVATE
    sc_config.cpp
)

add_subdirectory(game)
target_include_directories(scuffedcraft PRIVATE game)

add_subdirectory(renderer)
target_include_directories(scuffedcraft PRIVATE renderer)

add_subdirectory(engine)
target_include_directories(scuffedcraft PRIVATE engine)