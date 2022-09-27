#include "entry.h"

#include <algorithm>

namespace dcached {

char Entry::operator[](std::size_t n) const { return _data[n]; }
}  // namespace dcached
