#pragma once

#include "file_handler.h"
#include "constants.h"
#include "utility.h"

#include <iterator>
#include <map>
#include <optional>
#include <vector>
#include <string>
#include <fstream>

namespace {

template <typename T>
std::vector<char> map_to_vec(T&& container) {
  std::vector<char> buffer;
  for (auto& [key, value] : container) {
    for (char c : key) buffer.push_back(c);
    for (char c : value) buffer.push_back(c);
    buffer.push_back(',');
  }
  return buffer;
}

}  // namespace

namespace dcached {

template <typename K, typename V>
class MemTable {
 public:
  MemTable();

  std::optional<V> get(K const& key);
  void set(K const& key, V const& value);
  void del(K const& key);

  unsigned long size() const;

 private:
  void populate_from_log(std::string const& log_path);
  void dump_to_sstable();

  std::map<K, V> _container;
  FileManager _file_handler;

  unsigned long _data_size;
};

template <typename K, typename V>
MemTable<K, V>::MemTable() {
  populate_from_log(constants::outdir);
}

template <typename K, typename V>
void MemTable<K, V>::set(K const& key, V const& value) {
  auto record = util::concatenate(std::to_string(static_cast<int>(constants::ACTION::SET)), key, value);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.insert_or_assign(key, value);
}

template <typename K, typename V>
void MemTable<K, V>::del(K const& key) {
  auto record = util::concatenate(std::to_string(static_cast<int>(constants::ACTION::DEL)), key);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.erase(key);
}

template <typename K, typename V>
std::optional<V> MemTable<K, V>::get(K const& key) {
  auto const iter = _container.find(key);
  return iter != std::end(_container) ? iter->second : std::optional<V>{};
}

template <typename K, typename V>
void MemTable<K, V>::dump_to_sstable() {
  std::map<K, V> newmap;
  std::swap(_container, newmap);
  std::vector<char> buffer = map_to_vec(newmap);
  _file_handler.append_buffer(buffer);
}

template <typename K, typename V>
void MemTable<K, V>::populate_from_log(std::string const& log_path) {
  // THIS NO LONGER WORKS AS WE WRITE BYTES
  // INSTEAD OF COMMA SEPARATED STRINGS
  std::ifstream is{_file_handler.get_active_wal()};
  std::string record;
  while (std::getline(is, record, ',')) {
    auto [action, key, value] = util::split_key_value_record(record);
    switch (action) {
      case constants::ACTION::SET:
      _container.insert_or_assign(key, value);
      break;
      case constants::ACTION::DEL:
      _container.erase(key);
      break;
    }
  }
}

template <typename K, typename V>
unsigned long MemTable<K, V>::size() const {
  // TODO: actually track byte size
  return _container.size();
}

}  // namespace dcached
