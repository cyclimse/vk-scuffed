add_subdirectory(assets)
target_include_directories(scuffedcraft PRIVATE assets)

target_sources(scuffedcraft PRIVATE sc_renderer.cpp sc_window.cpp)
