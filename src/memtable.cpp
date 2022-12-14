#include "memtable.h"

#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "encoding.h"
#include "utility.h"

namespace {

std::size_t get_required_sz(const std::string& key, const std::string& value)
{
  return key.size() + value.size() + sizeof(dcached::constants::KVType) * 2;
}

template <typename T>
T& size_to_fit(const std::string& key, const std::string& value, T& container)
{
  using namespace dcached;
  auto min_req_sz = container.size() + get_required_sz(key, value);
  if (container.capacity() < min_req_sz)
    container.resize(container.capacity() * 1.6 + min_req_sz);
  return container;
}

char* pack_bin_record(const std::string& key, const std::string& value,
                      char* buf)
{
  using namespace dcached;
  std::uint64_t size_pack = pack_u32t_to_u64t(key.size(), value.size());
  buf = put_buf_u64t(size_pack, buf);
  buf = put_buf_string(key, buf);
  return put_buf_string(value, buf);
}

std::vector<char> map_to_vec(
    std::unordered_map<std::string, std::string>&& container)
{
  std::vector<char> buffer;
  for (auto& [key, value] : container) {
    std::vector<char> bin_record;
    pack_bin_record(key, value, &bin_record[0]);
    for (char c : bin_record) buffer.push_back(c);
  }
  return buffer;
}

std::istream& read_next(std::ifstream& is, std::string* key, std::string* value)
{
  using namespace dcached;

  // read & unpack size info
  char sbuf[sizeof(constants::KVType) * 2];
  if (!is.read(sbuf, sizeof(constants::KVType) * 2)) {
    std::cout << "failed to get size_pack\n";
    return is;
  }
  std::uint64_t size_pack = 0;
  get_buf_u64t(sbuf, &size_pack);
  auto [ksz, vsz] = unpack_u64t_to_u32t(size_pack);

  std::vector<char> kv_pack;
  kv_pack.resize(ksz + vsz);
  if (!is.read(&kv_pack[0], ksz + vsz)) {
    std::cout << "failed to get key_value_pack \n";
    return is;
  }

  auto ptr = &kv_pack[0];
  ptr = get_buf_string(ptr, ksz, key);
  ptr = get_buf_string(ptr, vsz, value);
  return is;
}

}  // namespace

namespace dcached {

MemTable::MemTable() { populate_from_log(_file_handler.get_active_wal()); }

void MemTable::set(std::string const& key, std::string const& value)
{
  std::string record;
  record.resize(get_required_sz(key, value));
  pack_bin_record(key, value, &record[0]);
  _file_handler.append_to_log(record.c_str(), record.size());
  _container.insert_or_assign(key, value);
  _size += record.size();
}

void MemTable::del(std::string const& key)
{
  const auto record = _container.find(key);
  if (record == _container.end()) return;
  std::string tmp, tombstone;
  tmp.resize(get_required_sz(key, tombstone));
  pack_bin_record(key, tombstone, &tmp[0]);
  _file_handler.append_to_log(tmp.c_str(), tmp.size());
  _size -= record->first.size() + record->second.size();
  _container.erase(key);
}

std::optional<std::string> MemTable::get(std::string const& key) const
{
  auto const iter = _container.find(key);
  return iter != std::end(_container) ? iter->second
                                      : std::optional<std::string>{};
}

void MemTable::dump_to_sstable()
{
  std::unordered_map<std::string, std::string> newmap;
  std::swap(_container, newmap);
  std::vector<char> buffer = map_to_vec(std::move(newmap));
  _file_handler.append_buffer(buffer);
}

void MemTable::populate_from_log(std::string const& log_path)
{
  std::ifstream is{log_path, std::ifstream::binary};
  while (true) {
    std::string key, value;
    if (!read_next(is, &key, &value))
      break;
    else if (value.size() > 0)
      _container.insert_or_assign(key, value);
    else
      _container.erase(key);
  }
}

unsigned long MemTable::size() const
{
  // TODO: actually track byte size
  // USE: entry.h
  return _container.size();
}

std::string MemTable::get_diagnostic_data() const
{
  std::stringstream ss;
  ss << "total_key_count: " << _container.size() << "\n"
     << "total_object_size: " << size() << "\n"
     << "current_log_file: " << _file_handler.get_active_wal() << "\n";
  return ss.str();
}

}  // namespace dcached
