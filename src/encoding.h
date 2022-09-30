#include <bitset>
#include <string>
#include <type_traits>
#include <cstring>
#include "constants.h"
#include <iostream>

namespace dcached {


template <typename T>
char* put_buf(const T* value, std::size_t size, char* out)
{
  std::memcpy(out, value, size);
  return out + size;
}

template <typename T>
char* get_buf(char* buf, std::size_t size, T* out)
{
  std::memcpy(out, buf, size);
  return buf + size;
}

char* put_buf_string(const std::string& key, char* out);

char* put_buf_u32t(std::uint32_t value, char* out);

char* put_buf_u64t(std::uint64_t value, char* out);

char* get_buf_string(char* buf, std::size_t size, std::string* out);

char* get_buf_u32t(char* buf, std::uint32_t* out);

char* get_buf_u64t(char* buf, std::uint64_t* out);

std::uint64_t pack_u32t_to_u64t(std::uint32_t a, std::uint32_t b);

std::pair<uint32_t, uint32_t> unpack_u64t_to_u32t(std::uint64_t n);


// manual encoding
template <typename T>
std::size_t encode(const T& value, std::size_t n, char *out) {
  std::memcpy(out, &value, 10);
  return n;
}

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
    num |= bin[i] == '1' ? 1 << (max_bit_index - i) : num;
  }
  return num;
}

template <typename T>
T binary_decode(const std::string &bin)
{
  return binary_decode<T>(bin.c_str(), bin.size());
}

std::string binary_encode(const char *buf, std::size_t size);

std::string binary_encode(const std::string &buf);

std::string binary_decode(const std::string &buf);

std::string binary_decode(const char *buf, std::size_t n);
}  // namespace dcached
