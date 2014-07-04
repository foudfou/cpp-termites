#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>


class Config
{
  static const int FILE_SIZE_MAX = 2048;
  static const int TMP_STR_MAX_LEN = 64;

  typedef char TmpString[TMP_STR_MAX_LEN];

  struct Entity {
    std::string species;
    int x;
    int y;
  };

public:
  Config();
  virtual ~Config();

  bool read(std::string const& configFile);

  void setTime(int t);
  void setWidth(int t);
  void setHeight(int t);
  void setChips(const std::map<std::string, int>& chps);
  void setSpecies(const std::map<std::string, std::map<std::string, int>>& spcs);
  bool checkSpecies(const std::map<std::string, std::map<std::string, int>>& spcs);

private:
  int time;
  int width;
  int height;
  // we store wood chips into a map, as wood species are supposely unique
  std::map<std::string, int> chips;
  std::map<std::string, std::map<std::string, int>> species;
  std::vector<Entity> termitePositions;
  std::vector<Entity> chipPositions;

  TmpString word;
  TmpString key;
  TmpString xcoord, ycoord;
  void extractToken(TmpString &dest, const char* &cur, const char* &start);
  void storeEntityPos(std::vector<Entity> &store, const TmpString &key,
                      const TmpString &x, const TmpString &y);

  bool parserHasError(int cs);
  bool parserIsFinished(int cs);
};

#endif /* _CONFIG_H_ */
