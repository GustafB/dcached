#pragma once

#include <map>
#include <optional>
#include <string>

#include "file_handler.h"

namespace dcached {

class MemTable {
public:
  MemTable();

  std::optional<std::string> get(std::string const& key);
  void set(const std::string& key, const std::string& value);
  void del(const std::string& key);
  unsigned long size() const;

private:
  void populate_from_log(const std::string& log_path);
  void dump_to_sstable();

  std::map<std::string, std::string> _container;
  FileManager _file_handler;
  unsigned long _data_size;
};

}  // namespace dcached
