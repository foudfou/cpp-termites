#include <iostream>

std::string trim(std::string const& source, char const* delims) {
  std::string result(source);
  std::string::size_type index;

  index = result.find_first_not_of(delims);
  if (index != std::string::npos)
    result.erase(0, index);

  index = result.find_last_not_of(delims);
  if (index != std::string::npos)
    result.erase(++index);
  else
    result.erase();

  return result;
}
