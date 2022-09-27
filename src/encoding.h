#include <string>
#include <type_traits>

namespace dcached {

std::string binary_encode(const char *buf, std::size_t size);

template <typename T,
          typename = typename std::enable_if<std::is_arithmetic_v<T>>::type>
std::string binary_encode(T n);

std::string binary_decode(std::string_view bin);

}  // namespace dcached
