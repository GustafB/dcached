#pragma once

#include "constants.h"
#include "utility.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <cassert>

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

class FileManager {
 public:
  FileManager()
      : _default_log{constants::outdir, std::fstream::in | std::fstream::out | std::fstream::app} {
    assert(_default_log.is_open());
  }

  // write routines
  template <int N>
  std::ofstream& append_buffer(std::array<char, N> buf);
  std::ofstream& append_buffer(std::vector<char> buf);
  std::ofstream& append_to_log(const char* buf, std::size_t buf_sz);

  // read routines

 private:
  std::ofstream& _append_to_file(const char* buf, std::size_t buf_sz, std::ofstream& file_handle);
  void _roll_log_file();

  const char* _current_log;
  std::ofstream _default_log;
  std::vector<std::string> _historical_logs;
};

template <int N>
std::ofstream& FileManager::append_buffer(std::array<char, N> buf) {
  return _append_to_file(buf.data(), buf.size(), std::ofstream{"/tmp/sstable.log"});
}

}  // namespace dcached
