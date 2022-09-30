#include "client.h"

#include <iostream>
#include <optional>
#include <string>
#include <tuple>

#include "constants.h"
#include "utility.h"

namespace {

bool validate_args(const std::vector<std::string> &args,
                   dcached::constants::ACTION action)
{
  switch (action) {
    case dcached::constants::ACTION::SET:
      return args.size() > 2;
    case dcached::constants::ACTION::DEL:
    case dcached::constants::ACTION::GET:
      return args.size() > 1;
    case dcached::constants::ACTION::DIAG:
      return true;
    default:
      return false;
  }
}

}  // namespace

namespace dcached {

void Client::run_loop()
{
  std::string stdin;
  while (std::getline(std::cin, stdin)) {
    auto tokens = util::tokenize(stdin);
    if (tokens.empty()) continue;
    auto action = util::stringToAction(tokens[0]);
    if (!validate_args(tokens, action)) continue;
    switch (action) {
      case constants::ACTION::SET:
        _memtable.set(tokens[1], tokens[2]);
        break;
      case constants::ACTION::DEL:
        _memtable.del(tokens[1]);
        break;
      case constants::ACTION::GET:
        std::cout << _memtable.get(tokens[1]).value_or("value not found")
                  << "\n";
        break;
      case constants::ACTION::DIAG:
        std::cout << _memtable.get_diagnostic_data() << "\n";
      default:
        continue;
    }
  }
}

}  // namespace dcached
