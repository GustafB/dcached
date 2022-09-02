#pragma once

#include "constants.h"
#include <sstream>
#include <string_view>

namespace dcached {
namespace util {

char const* actionToString(constants::ACTION action);
constants::ACTION stringToAction(std::string_view str);
constants::ACTION charToAction(char c);
unsigned long string_size(std::string const& str);

template <typename... Strings>
std::string concatenate(Strings&&... String) {
  return (std::string{} + ... + std::forward<Strings>(String));
}

}  // namespace util
}  // namespace dcached
