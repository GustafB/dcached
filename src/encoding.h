#include <bitset>
#include <string>
#include <type_traits>

#include "constants.h"

namespace dcached {

std::string binary_encode(const char *buf, std::size_t size);

std::string binary_encode(std::size_t n);

std::string binary_encode(const std::string &buf);

std::string binary_decode(std::string_view bin);

template <typename T>
std::string binary_encode(T n)
{
  static_assert(std::is_arithmetic_v<T>);
  return std::bitset<sizeof(T) * constants::ByteSz>(n).to_string();
}

template <typename T>
T binary_decode(std::string_view bin)
{
  static_assert(std::is_arithmetic<T>::value);
  T num = 0;
  short max_bit_index = sizeof(T) * 8 - 1;
  for (std::size_t i = 0; i < bin.size(); i++) {
    if (bin[i] == '1') num |= 1 << (max_bit_index - i);
  }
  return num;
}

}  // namespace dcached
