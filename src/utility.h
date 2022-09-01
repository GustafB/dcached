#include "constants.h"
#include <string_view>

namespace dcached {
const char *actionToString(constants::ACTION action);
constants::ACTION stringToAction(std::string_view str);
} // namespace dcached
