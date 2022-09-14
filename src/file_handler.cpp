#include "file_handler.h"

#include <fstream>
#include <cstdio>

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

std::string roll_wal() {
  auto new_log = constants::default_log;
  auto std::snprintf(new_log,
}

} // namespace dcached
