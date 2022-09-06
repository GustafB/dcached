#pragma once

#include "constants.h"
#include "utility.h"
#include <array>
#include <fstream>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace {

using Record = std::tuple<dcached::constants::ACTION, std::string, std::string>;

std::string compact_log(std::string const& old_path, std::string const& new_path) {
  std::ifstream old_log { old_path };
  std::ofstream new_log { new_path };
  std::unordered_map<std::string, Record> map;
  std::string record;
  while (std::getline(old_log, record)) {
    auto [action, key, value] = dcached::util::split_key_value_record(record);
    switch (action) {
      case dcached::constants::ACTION::SET:
        map.emplace(key, std::forward_as_tuple(action, key, value));
        break;
      case dcached::constants::ACTION::DEL:
        map.erase(key);
        break;
    }
  }
  for (auto& pair : map) {
    auto [action, key, value] = pair.second;
    new_log << dcached::util::join_key_value_record(action, key, value);
  }
  return "";
}

}  // namespace

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
  _append_to_file(buf.data(), _default_log);
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

template <typename T, typename V>
void FileManager<T, V>::_roll_log_file() {
  new_log = std::format(constants::default_log, 1);
}



}  // namespace dcached
