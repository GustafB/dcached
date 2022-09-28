#include <bitset>
#include <string>
#include <type_traits>

#include "constants.h"

namespace dcached {

std::string binary_encode(const char *buf, std::size_t size);

std::string binary_encode(const std::string &buf);

std::string binary_decode(const std::string &buf);

std::string binary_decode(const char *buf, std::size_t n);

template <typename T>
std::string binary_encode(T n)
{
  static_assert(std::is_arithmetic_v<T>);
  return std::bitset<sizeof(T) * constants::ByteSz>(n).to_string();
}

template <typename T>
T binary_decode(const char *bin, std::size_t n)
{
  static_assert(std::is_arithmetic<T>::value);
  T num = 0;
  short max_bit_index = sizeof(T) * constants::ByteSz - 1;
  for (std::size_t i = 0; i < n; i++) {
   if (bin[i] == '1') num |= 1 << (max_bit_index - i);
  }
  return num;
}

template <typename T>
T binary_decode(const std::string &bin)
{
  return binary_decode<T>(bin.c_str(), bin.size());
}

}  // namespace dcached
