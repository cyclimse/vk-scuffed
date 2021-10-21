#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <string>
#include <string_view>
using namespace std::string_view_literals;

namespace Constants {
inline constexpr std::uint32_t kVersion{1u};
inline constexpr std::string_view kProjectName{"ScuffedCraft"};
inline constexpr std::string_view kEngineName{"ScuffedEngine"};
#ifndef NDEBUG
inline constexpr std::string_view kReleaseName{"DEBUG"};
inline constexpr bool kEnableValidationLayer{true};
#else
inline constexpr std::string_view kReleaseName{"RELEASE"};
inline constexpr bool kEnableValidationLayer{false};
#endif
const std::string GetFullEngineName();
const std::string GetFullProjectName();
}  // namespace Constants

#endif  // CONSTANTS_HPP
