#include "utility.h"
#include "constants.h"
#include <string_view>

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



}  // namespace util
}  // namespace dcached
