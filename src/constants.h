#pragma once

#include <string>

namespace dcached {
namespace constants {

constexpr char sst_log[]{"/tmp/dcached_sst_%d"};
constexpr char default_log[]{"dcached.log"};
constexpr char outdir[]{"/tmp/dcached/dcached.log"};

enum class ACTION {
  SET = 0,
  GET = 1,
  DEL = 2,
  UNKNOWN = -1

};

}  // namespace constants
}  // namespace dcached
