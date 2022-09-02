#pragma once

#include "constants.h"
#include "utility.h"
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace dcached {

template <typename T, typename V>
class FileManager {
 public:
  FileManager()
      : _default_log{constants::outdir, std::fstream::in | std::fstream::out |
                     std::fstream::app} {}

  // write routines
  void append_to_log(constants::ACTION action, T const& key, V const& value);
  void append_to_log(constants::ACTION action, T const& key);
  template <int N>
  void append_buffer(std::array<char, N> buf);
  void append_buffer(std::vector<char> buf);

  // read routines

 private:
  void _append_to_file(std::string_view str, std::ofstream& file_handle);
  void _roll_log_file();

  std::ofstream _default_log;
  std::vector<std::string> _historical_logs;
};

template <typename T, typename V>
template <int N>
void FileManager<T, V>::append_buffer(std::array<char, N> buf) {
  _append_to_file(buf.data(), std::ofstream{"/tmp/sstable.log"});
}

template <typename T, typename V>
void FileManager<T, V>::append_buffer(std::vector<char> buf) {
  std::ofstream of{"/tmp/sstable.log"};
  _append_to_file(buf.data(), of);
}

template <typename T, typename V>
void FileManager<T, V>::append_to_log(constants::ACTION action, T const& key,
                                      V const& value) {
  auto record = util::concatenate(std::to_string(static_cast<int>(action)), "|",
                                  key, "|", value);
  _append_to_file(record, _default_log);
}

template <typename T, typename V>
void FileManager<T, V>::append_to_log(constants::ACTION action, T const& key) {
  auto record =
      util::concatenate(std::to_string(static_cast<int>(action)), "|", key);
  _append_to_file(record, _default_log);
}

template <typename T, typename V>
void FileManager<T, V>::_append_to_file(std::string_view str,
                                        std::ofstream& file_handle) {
  file_handle << str << ',';
}

}  // namespace dcached
