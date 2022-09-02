#pragma once

#include <map>
#include "file_handler.h"
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
