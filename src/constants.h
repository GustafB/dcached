#pragma once

#include <string>

namespace dcached {
namespace constants {

constexpr char outfile[]{"/tmp/test_file.txt"};

enum class ACTION {
  SET = 0,
  GET = 1,
  DEL = 2,
  UNKNOWN = -1

};

} // namespace constants
} // namespace dcached
