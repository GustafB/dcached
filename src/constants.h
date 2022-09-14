#pragma once

#include <string>

namespace dcached {
namespace constants {

constexpr char sst_log[]{"dcached_sst_%d.dat"};
constexpr char default_log[]{"dcached_%d.dat"};
constexpr char outdir[]{"/tmp/dcached/"};

enum class ACTION {
  SET = 0,
  GET = 1,
  DEL = 2,
  UNKNOWN = -1

};

}  // namespace constants
}  // namespace dcached
