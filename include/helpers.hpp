#ifndef _HELPERS_H_
#define _HELPERS_H_

long fileSize(const std::string& filename);

std::string trim(std::string const& source, char const* delims = " \t\r\n");

std::string extractParens(std::string const& source);

std::map<std::string, int>
tokenizeToMap(std::string const& source, char const* delims = " \t");

#endif /* _HELPERS_H_ */
