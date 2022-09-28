#include "encoding.h"

#include <bitset>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "constants.h"
#include <iostream>
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

std::string binary_decode(const char* bin, std::size_t n)
{
  std::string out;
  out.reserve(n / constants::ByteSz);
  for (std::size_t i = 0; i < n; i += constants::ByteSz) {
    out.push_back(binary_decode<unsigned char>(bin + i, constants::ByteSz));
  }
  return out;
}

std::string binary_encode(const char *buf, std::size_t n)
{
  std::string bin;
  bin.reserve(n * constants::ByteSz);
  for (std::size_t i = 0; i < n; ++i) {
    bin += binary_encode<unsigned char>(buf[i]);
  }
  return bin;
}

std::string binary_encode(const std::string &buf)
{
  return binary_encode(buf.c_str(), buf.size());
}

std::string binary_decode(const std::string &buf)
{
  return binary_decode(buf.c_str(), buf.size());
}

}  // namespace dcached
