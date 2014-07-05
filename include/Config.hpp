/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <list>
#include <map>
#include <string>
#include <vector>


class Config
{
  static const int TMP_STR_MAX_LEN = 64;
  static const int BUFFER_SIZE     = 1024;

  typedef std::map<std::string, int> Chips;
  typedef std::map<std::string, Chips> Species;
  typedef std::string TmpString;
  typedef std::list<std::string> TmpList;
  typedef Species TmpHash;

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
  void setChips(const Chips &chps);
  void setSpecies(const Species &spcs);
  bool checkSpecies(const Species &spcs);

private:
  int time;
  int width;
  int height;
  // we store wood chips into a map, as wood species are supposely unique
  std::map<std::string, int> chips;
  std::map<std::string, std::map<std::string, int>> species;
  std::vector<Entity> termitePositions;
  std::vector<Entity> chipPositions;

  struct ParserState {
    int cs;   // must remain persistent across chunk parsing runs
    const char *p;
    const char *pe;
    char *eof;

    int lineCount;
    bool fail;

    char buffer[BUFFER_SIZE];   // currently parserd chunk

    TmpString integer;
    TmpString word;
    TmpList   list;
    TmpString key;
    TmpHash   hash;
    TmpString xcoord, ycoord;
  };

  void storeEntityPos(std::vector<Entity> &store, const TmpString &key,
                      const TmpString &x, const TmpString &y);
  void parserInit(ParserState &parser);
  void parserExecute(ParserState &parser, const char *data, int len);
  bool parserFinish(ParserState &parser);
  bool parserHasError(ParserState &parser);
  bool parserIsFinished(ParserState &parser);
};

#endif /* _CONFIG_H_ */
