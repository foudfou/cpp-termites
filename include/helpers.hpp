#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <list>

long fileSize(const std::string& filename);

std::string btos(const bool& b);

std::string mapToString(const std::map<std::string, int>& m);

std::map<std::string, int> listToMap(std::list<std::string> l);

#endif /* _HELPERS_H_ */
