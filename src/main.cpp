#include <string>
#include "client.h"

int main() {
  dcached::Client client{};
  client.run_loop();
  return 0;
}
