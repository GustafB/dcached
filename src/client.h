#pragma once

#include <map>

#include "memtable.h"

namespace dcached {

class Client {
public:
  Client() = default;
  ~Client() = default;

  void run_loop();

private:
  MemTable<std::string, std::string> _memtable;
};

}  // namespace dcached
