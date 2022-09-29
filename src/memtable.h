#pragma once

#include <fstream>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "constants.h"
#include "encoding.h"
#include "file_handler.h"
#include "utility.h"

namespace {

std::istream& read_chunk(std::istream& is, std::string& output, std::size_t& offset, std::uint64_t chunk_size)
{
  output.resize(chunk_size);
  return is.read(&output[0], chunk_size);
}

std::istream& read_next(std::istream& is, std::string& output,
                         std::size_t& file_location)
{
  using namespace dcached;
  std::string size_buf;
  if (!read_chunk(is, size_buf, file_location, constants::MaxKVSize)) return is;
  constants::KVType vsz = dcached::binary_decode<constants::KVType>(size_buf.c_str(), constants::MaxKVSize) * 8;
  is.seekg(file_location += constants::MaxKVSize, std::ios_base::beg);

  std::string value_buf;
  if (!read_chunk(is, value_buf, file_location, vsz)) return is;
  is.seekg(file_location += vsz, std::ios_base::beg);
  output = dcached::binary_decode(value_buf);
  return is;
}

template <typename T>
std::vector<char> map_to_vec(T&& container)
{
  std::vector<char> buffer;
  for (auto& [key, value] : container) {
    auto bin = create_bin_record(key, value);
    for (char c : bin) buffer.push_back(c);
  }
  return buffer;
}

std::string create_bin_record(const std::string& key,
                              const std::string& value = "")
{
  return dcached::util::concatenate(
      dcached::binary_encode<dcached::constants::KVType>(key.size()),
      dcached::binary_encode(key),
      dcached::binary_encode<dcached::constants::KVType>(value.size()),
      dcached::binary_encode(value));
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
MemTable<K, V>::MemTable()
{
  populate_from_log(_file_handler.get_active_wal());
}

template <typename K, typename V>
void MemTable<K, V>::set(K const& key, V const& value)
{
  auto record = create_bin_record(key, value);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.insert_or_assign(key, value);
}

template <typename K, typename V>
void MemTable<K, V>::del(K const& key)
{
  auto record = create_bin_record(key);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.erase(key);
}

template <typename K, typename V>
std::optional<V> MemTable<K, V>::get(K const& key)
{
  auto const iter = _container.find(key);
  return iter != std::end(_container) ? iter->second : std::optional<V>{};
}

template <typename K, typename V>
void MemTable<K, V>::dump_to_sstable()
{
  std::map<K, V> newmap;
  std::swap(_container, newmap);
  std::vector<char> buffer = map_to_vec(newmap);
  _file_handler.append_buffer(buffer);
}

template <typename K, typename V>
void MemTable<K, V>::populate_from_log(std::string const& log_path)
{
  std::ifstream is{log_path, std::ifstream::binary};
  std::size_t offset = 0;
  while (is) {
    std::string key, value;
    read_next(is, key, offset);
    read_next(is, value, offset);
    std::cout << key << " " << value << "\n";
    if (value.size() > 0) {
      _container.insert_or_assign(key, value);
    } else {
      _container.erase(key);
    }
  }
}

template <typename K, typename V>
unsigned long MemTable<K, V>::size() const
{
  // TODO: actually track byte size
  // USE: entry.h
  return _container.size();
}

}  // namespace dcached
