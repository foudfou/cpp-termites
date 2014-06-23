#include <iostream>
#include <map>
#include "helpers.hpp"

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

std::string extractParens(std::string const& source) {
  std::string result(source);
  std::string::size_type index;

  index = result.find_first_not_of('(');
  if (index == std::string::npos)
  {
    std::cerr << "Missing '('" << std::endl;
    return result.erase();             // FIXME: should throw
  }
  result.erase(0, index+1);

  index = result.find_last_not_of(')');
  if (index == std::string::npos)
  {
    std::cerr << "Missing ')'" << std::endl;
    return result.erase();             // FIXME: should throw
  }
  result.erase(index);

  return result;
}

std::map<std::string, int>
tokenizeToMap(std::string const& source, char const* delims) {
  std::map<std::string, int> tokens;
  size_t  bgn = 0, end = -1;
  do
  {
    bgn = source.find_first_not_of(delims, bgn);
    end = source.find_first_of(delims, bgn);
    std::string tok = source.substr(bgn, end - bgn);
    if (tokens[tok])
    {
      std::cerr << "Duplicate token ignored: " << tok << std::endl;
    }
    else
    {
      ++tokens[tok];
    }
    bgn = end;
  }
  while (end != std::string::npos);
  return tokens;
}
