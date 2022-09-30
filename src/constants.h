#pragma once

#include <string>

namespace dcached {
namespace constants {

constexpr char sst_log[]{"dcached_sst_%d.dat"};
constexpr char default_log[]{"dcached_%d.dat"};
constexpr char outdir[]{"/tmp/dcached/"};

constexpr std::size_t ByteSz = 8;

using KVType = std::uint32_t;

constexpr std::size_t MaxKVSize = sizeof(KVType) * ByteSz;

enum class ACTION { SET = 0, GET = 1, DEL = 2, DIAG = 3, UNKNOWN = -1 };

}  // namespace constants
}  // namespace dcached
