#include "memtable.h"

#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "constants.h"
#include "encoding.h"
#include "utility.h"

namespace {

std::istream& read_chunk(std::istream& is, std::size_t offset,
                         std::uint64_t chunk_size, std::string* output)
{
  output->resize(chunk_size);
  return is.read(&(*output)[0], chunk_size);
}

std::istream& read_next(std::istream& is, std::string* output,
                        std::size_t* offset)
{
  using namespace dcached;
  std::string size_buf;
  if (!read_chunk(is, *offset, constants::MaxKVSize, &size_buf)) return is;
  constants::KVType vsz = dcached::binary_decode<constants::KVType>(
                              size_buf.c_str(), constants::MaxKVSize) *
                          constants::ByteSz;
  is.seekg(*offset += constants::MaxKVSize, std::ios_base::beg);

  std::string value_buf;
  if (!read_chunk(is, *offset, vsz, &value_buf)) return is;
  is.seekg(*offset += vsz, std::ios_base::beg);
  *output = dcached::binary_decode(value_buf);
  return is;
}

std::string create_bin_record(const std::string& key,
                              const std::string& value = "")
{
  using namespace dcached;
  return dcached::util::concatenate(
      binary_encode<dcached::constants::KVType>(key.size()), binary_encode(key),
      binary_encode<dcached::constants::KVType>(value.size()),
      binary_encode(value));
}

std::vector<char> map_to_vec(std::map<std::string, std::string>&& container)
{
  std::vector<char> buffer;
  for (auto& [key, value] : container) {
    auto bin = create_bin_record(key, value);
    for (char c : bin) buffer.push_back(c);
  }
  return buffer;
}

}  // namespace

namespace dcached {

MemTable::MemTable() { populate_from_log(_file_handler.get_active_wal()); }

void MemTable::set(std::string const& key, std::string const& value)
{
  auto record = create_bin_record(key, value);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.insert_or_assign(key, value);
}

void MemTable::del(std::string const& key)
{
  auto record = create_bin_record(key);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.erase(key);
}

std::optional<std::string> MemTable::get(std::string const& key)
{
  auto const iter = _container.find(key);
  return iter != std::end(_container) ? iter->second
                                      : std::optional<std::string>{};
}

void MemTable::dump_to_sstable()
{
  std::map<std::string, std::string> newmap;
  std::swap(_container, newmap);
  std::vector<char> buffer = map_to_vec(std::move(newmap));
  _file_handler.append_buffer(buffer);
}

void MemTable::populate_from_log(std::string const& log_path)
{
  std::ifstream is{log_path, std::ifstream::binary};
  std::size_t offset = 0;
  while (is) {
    std::string key, value;
    read_next(is, &key, &offset);
    read_next(is, &value, &offset);
    std::cout << key << " " << value << "\n";
    if (value.size() > 0) {
      _container.insert_or_assign(key, value);
    }
    else {
      _container.erase(key);
    }
  }
}

unsigned long MemTable::size() const
{
  // TODO: actually track byte size
  // USE: entry.h
  return _container.size();
}

}  // namespace dcached
