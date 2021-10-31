#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
using namespace std::string_view_literals;

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Constants {
inline constexpr std::uint32_t kVersion{1u};
inline constexpr std::string_view kProjectName{"ScuffedCraft"};
inline constexpr std::string_view kEngineName{"ScuffedEngine"};
#ifndef NDEBUG
inline constexpr std::string_view kReleaseName{"DEBUG"};
#else
inline constexpr std::string_view kReleaseName{"RELEASE"};
#endif
namespace Engine {
#ifndef NDEBUG
inline constexpr std::array<const char*, 1> kDeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
inline constexpr bool kEnableValidationLayer{true};
#else
inline constexpr std::string_view kReleaseName{"RELEASE"};
inline constexpr bool kEnableValidationLayer{false};
#endif
}  // namespace Engine
const std::string GetFullEngineName();
const std::string GetFullProjectName();
}  // namespace Constants

#endif  // CONSTANTS_HPP
