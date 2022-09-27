#pragma once

#include <map>
#include <string>
#include <vector>

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
