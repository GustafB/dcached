#include "file_handler.h"

#include <fstream>
#include <cstdio>

namespace {

int parse_log_number(const std::string& log_file) {
  // we already know log format so
  // no need for this to be generic
  auto b = log_file.find_last_of('_') + 1;
  auto e = log_file.find_last_of('.');
  return std::atoi(log_file.substr(b, e - b).c_str());
}

}

namespace dcached {

std::ofstream& FileManager::append_to_log(const char* buf, std::size_t buf_sz) {
  return _append_to_file(buf, buf_sz, _default_log);
}

std::ofstream& FileManager::_append_to_file(const char* buf,
                                            std::size_t buf_sz,
                                            std::ofstream& file_handle) {
  file_handle.write(buf, buf_sz);
  return file_handle;
}

std::string roll_wal(const std::string& current_log, const char* fmt) {
  char new_log[100];
  std::snprintf(new_log, sizeof(new_log), fmt, parse_log_number(current_log) + 1);
  return new_log;
}

} // namespace dcached
