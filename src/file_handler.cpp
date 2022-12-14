#include "file_handler.h"

#include <cstdio>
#include <fstream>

namespace dcached {

std::ofstream& FileManager::append_to_log(const char* buf, std::size_t buf_sz)
{
  return _append_to_file(buf, buf_sz, _default_log);
}

std::ofstream& FileManager::_append_to_file(const char* buf, std::size_t buf_sz,
                                            std::ofstream& file_handle)
{
  file_handle.write(buf, buf_sz);
  file_handle.flush();
  return file_handle;
}

std::string FileManager::roll_wal(const std::string& current_log,
                                  const char* fmt)
{
  return util::generate_log_file_name(fmt, util::parse_log_number(current_log));
}

std::string FileManager::get_active_wal() const { return _current_log; }

std::ofstream& FileManager::append_buffer(std::vector<char> buf)
{
  auto sst = std::ofstream{"/tmp/sstable.log"};
  return _append_to_file(buf.data(), buf.size(), sst);
}

}  // namespace dcached
