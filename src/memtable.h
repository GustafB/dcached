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

std::ifstream& read_next(std::ifstream& is, std::string& output,
                         std::size_t& file_location)
{
  constexpr std::size_t uint64_bitsize = sizeof(std::uint64_t) * 8;
  char size_buf[uint64_bitsize];
  is.read(size_buf, uint64_bitsize);
  std::size_t value_size = dcached::binary_decode<std::size_t>({size_buf}) * 8;
  is.seekg(file_location += uint64_bitsize, std::ios_base::beg);
  std::string value_buf;
  value_buf.resize(value_size);
  is.read(&value_buf[0], value_size);
  is.seekg(file_location += value_size, std::ios_base::beg);
  output = dcached::binary_decode(value_buf);
  return is;
}

template <typename T>
std::vector<char> map_to_vec(T&& container)
{
  std::vector<char> buffer;
  for (auto& [key, value] : container) {
    for (char c : key) buffer.push_back(c);
    for (char c : value) buffer.push_back(c);
    buffer.push_back(',');
  }
  return buffer;
}

std::string create_bin_record(dcached::constants::ACTION action,
                              const std::string& key,
                              const std::string& value = "")
{
  auto key_b = dcached::binary_encode(key);
  auto value_b = dcached::binary_encode(value);
  auto key_s = dcached::binary_encode<std::size_t>(key.size());
  auto value_s = dcached::binary_encode<std::size_t>(value.size());
  return dcached::util::concatenate(key_s, key_b, value_s, value_b);
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
  populate_from_log(constants::outdir);
}

template <typename K, typename V>
void MemTable<K, V>::set(K const& key, V const& value)
{
  // auto record = util::concatenate(
  //     std::to_string(static_cast<int>(constants::ACTION::SET)), key, value);
  auto record = create_bin_record(constants::ACTION::SET, key, value);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.insert_or_assign(key, value);
}

template <typename K, typename V>
void MemTable<K, V>::del(K const& key)
{
  auto record = create_bin_record(constants::ACTION::SET, key);
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
  // THIS NO LONGER WORKS AS WE WRITE BYTES
  // INSTEAD OF COMMA SEPARATED STRINGS
  std::ifstream is{_file_handler.get_active_wal(),
                   std::ifstream::ate | std::ifstream::binary};
  auto file_size = is.tellg();
  std::size_t file_location = 0;
  is.seekg(file_location);
  while (file_location < file_size) {
    std::string key, value;
    read_next(is, key, file_location);
    read_next(is, value, file_location);
    std::cout << key << " " << value << "\n";
  }

  // while (std::getline(is, record, ',')) {
  //   auto [action, key, value] = util::split_key_value_record(record);
  //   switch (action) {
  //     case constants::ACTION::SET:
  //       _container.insert_or_assign(key, value);
  //       break;
  //     case constants::ACTION::DEL:
  //       _container.erase(key);
  //       break;
  //   }
  // }
}

template <typename K, typename V>
unsigned long MemTable<K, V>::size() const
{
  // TODO: actually track byte size
  // USE: entry.h
  return _container.size();
}

}  // namespace dcached
