#pragma once

#include <iterator>
#include <map>
#include <optional>
#include "file_handler.h"

namespace dcached {

template <typename K, typename V>
class MemTable {
 public:
  MemTable();

  std::optional<V> get(const K& key);
  void set(const K& key, const V& value);
  void del(const K& key);

  unsigned long size() const;

 private:
  std::vector<char> dump_to_vec(std::map<K, V>& container);
  void populate_from_log(const std::string& log_path);

  std::map<K, V> _container;
  FileManager<K, V> _file_handler;
};

template <typename K, typename V>
MemTable<K, V>::MemTable() {
  populate_from_log(constants::outdir);
}

template <typename K, typename V>
void MemTable<K, V>::set(const K& key, const V& value) {
  _file_handler.append_to_log(constants::ACTION::SET, key, value);
  _container.insert_or_assign(key, value);
}

template <typename K, typename V>
void MemTable<K, V>::del(const K& key) {
  _container.erase(key);
  _file_handler.append_to_log(constants::ACTION::DEL, key);
}

template <typename K, typename V>
std::optional<V> MemTable<K, V>::get(const K& key) {
  const auto iter = _container.find(key);
  return iter != std::end(_container) ? iter->second : std::optional<V>{};
}

template <typename K, typename V>
std::vector<char> MemTable<K, V>::dump_to_vec(std::map<K, V>& container) {
  std::vector<char> chars;
  for (auto& [key, value] : container) {
    for (char c : key)
      chars.push_back(c);
    for (char c : value)
      chars.push_back(c);
  }
  return chars;
}

template <typename K, typename V>
void MemTable<K, V>::populate_from_log(const std::string& log_path) {
  // TODO: need smarter logic for this...
  // TODO: add record byte size to log
  //       and use seekg
  std::ifstream is{log_path};
  std::string record;
  while (std::getline(is, record, ',')) {
    auto e1 = record.find('|', 2);
    auto e2 = record.find('|', e1);
    auto key = std::string{std::begin(record) + 2, std::begin(record) + e1};
    auto value = std::string{std::begin(record) + e1 + 1, std::end(record)};
    _container.insert_or_assign(key, value);
  }
}

template <typename K, typename V>
unsigned long MemTable<K, V>::size() const {
  // TODO: actually track byte size
  return _container.size();
}

}  // namespace dcached
