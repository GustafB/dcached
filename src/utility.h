#pragma once

#include "constants.h"
#include <sstream>
#include <string_view>
#include <tuple>

namespace dcached {
namespace util {

char const* actionToString(constants::ACTION action);
constants::ACTION stringToAction(std::string_view str);
constants::ACTION charToAction(char c);
unsigned long string_size(std::string const& str);

template <typename... Strs>
std::string concatenate(Strs&&... String) {
  return (std::string{} + ... + std::forward<Strs>(String));
}

std::tuple<constants::ACTION, std::string, std::string> split_key_value_record(const std::string& str);

std::string join_key_value_record(constants::ACTION action, const std::string& key, const std::string& value);

int parse_log_number(const std::string& log_file);

std::string generate_log_file_name(const char* fmt, int n);

}  // namespace util
}  // namespace dcached
