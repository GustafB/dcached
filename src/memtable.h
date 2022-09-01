#include "file_handler.h"
#include <map>
#include <optional>

namespace dcached {

template <typename K, typename V>
class MemTable {
public:
  std::optional<V> get(const K &key);
  void set(const K &key, const V &value);
  void del(const K &key);

private:
  std::vector<char> dump_to_vec(std::map<K,V>& container);

  std::map<K, V> _container;
  FileManager<K, V> _file_handler;
  unsigned long _byte_size;
};

template <typename K, typename V>
void MemTable<K, V>::set(const K &key, const V &value) {
  _file_handler.append_to_log(constants::ACTION::SET, key, value);
  _container.insert_or_assign(key, value);
}

template <typename K, typename V> void MemTable<K, V>::del(const K &key) {
  _container.erase(key);
  _file_handler.append_to_log(constants::ACTION::DEL, key);
}

template <typename K, typename V>
std::optional<V> MemTable<K, V>::get(const K &key) {
  const auto iter = _container.find(key);
  return iter != std::end(_container) ? iter->second : std::optional<V>{};
}

template <typename K, typename V>
std::vector<char> MemTable<K, V>::dump_to_vec(std::map<K, V>& container) {
  std::vector<char> chars;
  for (auto& [key, value] : container) {
    for (char c: key) chars.push_back(c);
    for (char c: value) chars.push_back(c);
  }
  return chars;
}

} // namespace dcached
