{ pkgs ? (import <nixpkgs> {}) }:
with pkgs;
mkShell {
  nativeBuildInputs = [
    # IDE
    vscodium-fhs
    clang-tools
    # Build tools
    cmake 
    clang_12
    lldb_12
    shaderc
    # Debug tools
    cppcheck
    vulkan-validation-layers
    # Libs
    makeWrapper
    vulkan-headers
    glfw
  ];

  buildInputs = [
    vulkan-loader
  ];

  # If it doesn’t get picked up through nix magic
  VULKAN_SDK = "${vulkan-validation-layers}/share/vulkan/explicit_layer.d";
  LD_LIBRARY_PATH="/run/opengl-driver/lib:/run/opengl-driver-32/lib";
}