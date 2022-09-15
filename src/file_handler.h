#pragma once

#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "utility.h"

namespace {

using Record = std::tuple<dcached::constants::ACTION, std::string, std::string>;
namespace fs = std::filesystem;

std::string compact_log(std::string const& old_path,
                        std::string const& new_path)
{
  std::ifstream old_log{old_path};
  std::ofstream new_log{new_path};
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

std::string initialize_log_dir()
{
  const fs::path system_dir_path{dcached::constants::outdir};
  if (!fs::exists(system_dir_path)) {
    fs::create_directory(system_dir_path);
  }
  return system_dir_path.string();
}

std::string find_current_log(const std::string& log_fmt)
{
  initialize_log_dir();
  auto cmpend = log_fmt.find_last_of('_');
  auto cmpstart = sizeof(dcached::constants::outdir) - 1;
  auto cmpstr = log_fmt.substr(0, log_fmt.find_last_of('_'));
  const fs::path system_dir_path{dcached::constants::outdir};
  int log_file_number = 0;
  for (auto& log_file : fs::directory_iterator{system_dir_path}) {
    auto file_name = log_file.path().string();
    auto cmp =
        file_name.substr(cmpstart, file_name.find_last_of('_') - cmpstart);
    if (cmp == cmpstr) {
      log_file_number =
          std::max(log_file_number, dcached::util::parse_log_number(file_name));
    }
  }
  return dcached::util::generate_log_file_name(log_fmt.c_str(),
                                               log_file_number);
}

}  // namespace

namespace dcached {

class FileManager {
public:
  FileManager()
      : _current_log{constants::outdir +
                     find_current_log(constants::default_log)}
  {
    _default_log =
        std::ofstream{_current_log, std::fstream::out | std::fstream::app};
    assert(_default_log.is_open());
  }

  // write routines
  template <int N>
  std::ofstream& append_buffer(std::array<char, N> buf);
  std::ofstream& append_buffer(std::vector<char> buf);
  std::ofstream& append_to_log(const char* buf, std::size_t buf_sz);

  // read routines

  // utils
  std::string get_active_wal();

private:
  std::ofstream& _append_to_file(const char* buf, std::size_t buf_sz,
                                 std::ofstream& file_handle);
  std::string roll_wal(const std::string& current_log, const char* fmt);

  std::string _current_log;
  std::ofstream _default_log;
};

template <int N>
std::ofstream& FileManager::append_buffer(std::array<char, N> buf)
{
  return _append_to_file(buf.data(), buf.size(),
                         std::ofstream{"/tmp/sstable.log"});
}

}  // namespace dcached
