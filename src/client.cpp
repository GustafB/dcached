#include "client.h"
#include "constants.h"
#include "file_handler.h"
#include "utility.h"
#include <iostream>
#include <optional>
#include <tuple>

namespace {

std::tuple<dcached::constants::ACTION, std::string, std::string> get_user_input(
    std::istream& is) {
  std::string action;
  std::string key;
  std::string data;
  std::cout << "input action {GET, SET, DEL} {KEY} {VALUE} ";
  is >> action;
  is >> key;
  is >> data;
  return {dcached::util::stringToAction(action), key, data};
}

}  // namespace

namespace dcached {

void Client::run_loop() {
  while (std::cin) {
    auto [action, key, value] = get_user_input(std::cin);
    switch (action) {
      case constants::ACTION::SET:
        _memtable.set(key, value);
        break;
      case constants::ACTION::DEL:
        _memtable.del(key);
        break;
      case constants::ACTION::GET:
        std::cout << _memtable.get(key).value_or("value not found") << "\n";
        break;
      case constants::ACTION::UNKNOWN:
        continue;
        break;
    }
  }
}

}  // namespace dcached
