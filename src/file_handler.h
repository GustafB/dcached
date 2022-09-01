#pragma once

#include "constants.h"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace dcached {

template <typename T, typename V> class FileManager {
public:
  FileManager()
      : _default_log{constants::outfile, std::fstream::in | std::fstream::out |
                                             std::fstream::app} {}
  void append_to_log(constants::ACTION action, const T &key, const V &value);
  void append_to_log(constants::ACTION action, const T &key);
  template <int N> void dump_buffer(std::array<char, N> buf);

private:
  void _append_to_file(std::string_view str, std::ofstream &file_handle);
  void _roll_log_file();

  std::ofstream _default_log;
  std::vector<std::string> _historical_logs;
};

template <typename T, typename V>
template <int N>
void FileManager<T, V>::dump_buffer(std::array<char, N> buf) {
  _append_to_file(buf.data());
}

template <typename T, typename V>
void FileManager<T, V>::append_to_log(constants::ACTION action, const T &key,
                                      const V &value) {
  _append_to_file(std::to_string(static_cast<int>(action)) + "|" + key + "|" +
                      value,
                  _default_log);
}

template <typename T, typename V>
void FileManager<T, V>::append_to_log(constants::ACTION action, const T &key) {
  _append_to_file(std::to_string(static_cast<int>(action)) + "|" + key,
                  _default_log);
}

template <typename T, typename V>
void FileManager<T, V>::_append_to_file(std::string_view str,
                                        std::ofstream &file_handle) {
  file_handle << str << ',';
}

} // namespace dcached
