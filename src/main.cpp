#include "client.h"
#include <string>

int main() {
  dcached::Client client{};
  client.run_loop();
  return 0;
}
