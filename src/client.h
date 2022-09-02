#pragma once

#include "file_handler.h"
#include "memtable.h"
#include <map>

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
