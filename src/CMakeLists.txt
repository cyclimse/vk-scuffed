target_sources(scuffedcraft PRIVATE sc_config.cpp)

add_subdirectory(assets)
target_include_directories(scuffedcraft PRIVATE assets)

add_subdirectory(game)
target_include_directories(scuffedcraft PRIVATE game)

add_subdirectory(renderer)
target_include_directories(scuffedcraft PRIVATE renderer)

add_subdirectory(engine)
target_include_directories(scuffedcraft PRIVATE engine)

add_subdirectory(utils)
target_include_directories(scuffedcraft PRIVATE utils)
