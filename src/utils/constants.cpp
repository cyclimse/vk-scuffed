#include "constants.hpp"

const std::string Constants::GetFullEngineName() {
  return std::string{Constants::kEngineName} + "_" +
         std::to_string(Constants::kVersion) + "_" +
         Constants::kReleaseName.data();
}

const std::string Constants::GetFullProjectName() {
  return std::string{Constants::kProjectName} + "_" +
         std::to_string(Constants::kVersion) + "_" +
         Constants::kReleaseName.data();
}