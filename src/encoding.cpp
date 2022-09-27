#include "encoding.h"

#include <bitset>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "constants.h"

namespace {

std::uint32_t pack_ints(std::uint16_t a, std::uint16_t b)
{
  return (static_cast<std::uint32_t>(a) << 16) | b;
}

std::pair<uint16_t, uint16_t> unpack_ints(std::uint32_t n)
{
  return {static_cast<std::uint16_t>(n >> 16), static_cast<std::uint16_t>(n)};
}

}  // namespace

namespace dcached {

std::string binary_encode(const char *buf, std::size_t size)
{
  std::string bin;
  bin.reserve(size * constants::ByteSz);
  for (std::size_t i = 0; i < size; ++i) {
    bin += std::bitset<constants::ByteSz>(buf[i]).to_string();
  }
  return bin;
}

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic_v<T>>::type>
std::string binary_encode(T n)
{
  return std::bitset<sizeof(T) * constants::ByteSz>(n).to_string();
}

std::string binary_decode(std::string_view bin)
{
  std::string out;
  out.reserve(bin.size() / constants::ByteSz);
  for (std::size_t i = 0; i < bin.size(); i += 8) {
    unsigned char c = 0;
    for (std::size_t j = 0; j < 8 ; ++j) {
      if (bin[i + j] == '1') c |= 1 << (7 - j);
    }
    out.push_back(c);
  }
  return out;
}



}  // namespace dcached
