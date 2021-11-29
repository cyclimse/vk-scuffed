{ pkgs ? (import <nixpkgs> {}) }:
with pkgs;
mkShell {
  nativeBuildInputs = [
    # IDE
    vscodium-fhs
    clang-tools
    pre-commit
    # Build tools
    cmake
    clang_12
    lldb_12
    gdb
    # Debug tools
    cmake-format
    cppcheck
    include-what-you-use
    vulkan-validation-layers
    # Libs
    makeWrapper
    vulkan-headers
    glm
    boost175
    glfw
  ];

  buildInputs = [
    vulkan-loader
  ];

  # If it doesn’t get picked up through nix magic
  VULKAN_SDK = "${vulkan-validation-layers}/share/vulkan/explicit_layer.d";
  SHADERC_LIB = "${shaderc}/lib";
  LD_LIBRARY_PATH = "/run/opengl-driver/lib:/run/opengl-driver-32/lib";
  LSAN_OPTIONS = "suppressions=asan.supp";
}
