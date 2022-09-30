#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "file_handler.h"

namespace dcached {

class MemTable {
public:
  MemTable();

  std::optional<std::string> get(std::string const& key) const;
  void set(const std::string& key, const std::string& value);
  void del(const std::string& key);
  std::string get_diagnostic_data() const;
  unsigned long size() const;

private:
  void populate_from_log(const std::string& log_path);
  void dump_to_sstable();

  std::unordered_map<std::string, std::string> _container;
  FileManager _file_handler;
  unsigned long _size;
};

}  // namespace dcached
