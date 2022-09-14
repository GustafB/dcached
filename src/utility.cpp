#include "utility.h"
#include "constants.h"
#include <string_view>
#include <tuple>

namespace dcached {
namespace util {
char const* actionToString(constants::ACTION action) {
  switch (action) {
    case constants::ACTION::SET:
      return "SET";
    case constants::ACTION::GET:
      return "GET";
    case constants::ACTION::DEL:
      return "DEL";
    default:
      return "UNKNOWN";
  }
}

constants::ACTION charToAction(char c) {
  switch (c) {
    case '0':
      return constants::ACTION::SET;
    case '1':
      return constants::ACTION::DEL;
    case '2':
      return constants::ACTION::GET;
    default:
      return constants::ACTION::UNKNOWN;
  }
}

constants::ACTION stringToAction(std::string_view str) {
  return   str == "SET" ? constants::ACTION::SET
      : str == "GET" ? constants::ACTION::GET
      : str == "DEL" ? constants::ACTION::DEL
      : constants::ACTION::UNKNOWN;
}

std::tuple<constants::ACTION, std::string, std::string>
split_key_value_record(std::string const& record) {
  auto action = charToAction(record[0]);
  auto dpos = record.find('|', 2);
  auto key = std::string{std::begin(record) + 2, std::begin(record) + dpos};
  auto value = std::string{std::begin(record) + dpos + 1, std::end(record)};
  return { action, key, value };
}

std::string join_key_value_record(constants::ACTION action, const std::string& key,
                                  const std::string& value) {
  return concatenate(actionToString(action), "|", key, "|", value);
}

int parse_log_number(const std::string& log_file) {
  // we already know log format so
  // no need for this to be generic
  auto b = log_file.find_last_of('_') + 1;
  auto e = log_file.find_last_of('.');
  return std::atoi(log_file.substr(b, e - b).c_str());
}


std::string generate_log_file_name(const char* fmt, int n) {
  char new_log[100];
  std::snprintf(new_log, sizeof(new_log), fmt, n);
  return new_log;
}


}  // namespace util
}  // namespace dcached
