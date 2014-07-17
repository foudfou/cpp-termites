/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <list>
#include <map>
#include <string>
#include <vector>


class Config
{
public:
  struct Entity {
    std::string species;
    int row;
    int col;

    bool operator==(const Entity& rhs) const;
  };

  typedef std::map<std::string, int>   Chips;
  typedef std::map<std::string, Chips> Species;
  typedef std::vector<Entity>          Positions;

  Config();
  virtual ~Config();

  bool read(std::string const& configFile);
  bool check() const;
  bool checkParamsDefined() const;
  bool checkSpeciesChipsCoherence() const;
  bool checkSpeciesAndBounds() const;
  bool checkInitialPositions() const;

  int  getTics() const;
  void setTics(int t);
  int  getWidth() const;
  void setWidth(int t);
  int  getHeight() const;
  void setHeight(int t);
  Chips getChips() const;
  void setChips(const Chips &chps);
  Species getSpecies() const;
  void setSpecies(const Species &spcs);
  Positions getTermitePositions() const;
  void setTermitePositions(const Positions &tpos);
  Positions getChipPositions() const;
  void setChipPositions(const Positions &cpos);

private:
  static const int TMP_STR_MAX_LEN = 64;
  static const int BUFFER_SIZE     = 1024;

  typedef std::string TmpString;
  typedef std::list<std::string> TmpList;
  typedef Species TmpHash;

  int tics;
  int width;
  int height;
  // we store wood chips into a map, as wood species are supposely unique
  Chips chips;
  Species species;
  Positions termitePositions;
  Positions chipPositions;

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

  void storeEntityPosition(Positions &store, const TmpString &key,
                           const TmpString &x, const TmpString &y);
  void parserInit(ParserState &parser);
  void parserExecute(ParserState &parser, const char *data, int len);
  bool parserFinish(ParserState &parser);
  bool parserHasError(ParserState &parser);
  bool parserIsFinished(ParserState &parser);
};

#endif /* _CONFIG_H_ */
