#include "constants.h"
#include <string_view>

namespace dcached {

const char *actionToString(constants::ACTION action) {
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

constants::ACTION stringToAction(std::string_view str) {
  return str == "SET"   ? constants::ACTION::SET
         : str == "GET" ? constants::ACTION::GET
         : str == "DEL" ? constants::ACTION::DEL
                        : constants::ACTION::UNKNOWN;
}

} // namespace dcached
