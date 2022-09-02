#pragma once

#include <sstream>
#include <string_view>
#include "constants.h"

namespace dcached {
namespace util {

const char* actionToString(constants::ACTION action);
constants::ACTION stringToAction(std::string_view str);
unsigned long string_size(const std::string& str);

template <typename... Strings>
std::string concatenate(Strings&&... String) {
  return (std::string{} + ... + std::forward<Strings>(String));
}



}  // namespace util
}  // namespace dcached
