#pragma once

#include <string>
#include <string_view>

namespace dcached {

struct Entry {
  Entry() = delete;
  Entry(const char* s, std::size_t n) : _data{s}, _size{n} {}
  Entry(const std::string& s) : _data{s.data()}, _size{s.size()} {}
  Entry(const std::string_view s) : _data{s.data()}, _size{s.size()} {}

  char operator[](std::size_t n) const;

  const char* data() const { return _data; }
  std::size_t size() const { return _size; }

private:
  const char* _data;
  std::size_t _size;
};

std::string binary_encode(const char* buf, std::size_t size);
std::string binary_encode(std::uint32_t n);

}  // namespace dcached
