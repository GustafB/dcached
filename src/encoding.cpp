#include "encoding.h"

#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

#include "constants.h"

namespace dcached {

char* put_buf_string(const std::string& key, char* out)
{
  return put_buf(&key[0], key.size(), out);
}

char* put_buf_u32t(std::uint32_t value, char* out)
{
  std::size_t sz = sizeof(value);
  return put_buf(&value, sz, out);
}

char* put_buf_u64t(std::uint64_t value, char* out)
{
  std::size_t sz = sizeof(value);
  return put_buf(&value, sz, out);
}

char* get_buf_string(char* buf, std::size_t size, std::string* out)
{
  out->resize(size);
  get_buf(buf, size, &(*out)[0]);
  return buf + size;
}

char* get_buf_u32t(char* buf, std::uint32_t* out)
{
  return get_buf(buf, sizeof(std::uint32_t), out);
}

char* get_buf_u64t(char* buf, std::uint64_t* out)
{
  return get_buf(buf, sizeof(std::uint64_t), out);
}

std::uint64_t pack_u32t_to_u64t(std::uint32_t a, std::uint32_t b)
{
  return (static_cast<std::uint64_t>(a) << 32) | b;
}

std::pair<uint32_t, uint32_t> unpack_u64t_to_u32t(std::uint64_t n)
{
  return {static_cast<std::uint32_t>(n >> 32), static_cast<std::uint32_t>(n)};
}

// manual encodiding & decoding
std::string binary_decode(const char* bin, std::size_t n)
{
  std::string out;
  out.reserve(n / constants::ByteSz);
  for (std::size_t i = 0; i < n; i += constants::ByteSz) {
    out.push_back(binary_decode<unsigned char>(bin + i, constants::ByteSz));
  }
  return out;
}

std::string binary_encode(const char* buf, std::size_t n)
{
  std::string bin;
  bin.reserve(n * constants::ByteSz);
  for (std::size_t i = 0; i < n; ++i) {
    bin += binary_encode<unsigned char>(buf[i]);
  }
  return bin;
}

std::string binary_encode(const std::string& buf)
{
  return binary_encode(buf.c_str(), buf.size());
}

std::string binary_decode(const std::string& buf)
{
  return binary_decode(buf.c_str(), buf.size());
}

}  // namespace dcached
