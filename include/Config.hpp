/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <istream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "helpers.hpp"

/** Class for parsing configuration files. */
class Config
{
public:
  /** Virtual pieces intended to exist on the Board. */
  struct Entity {
    std::string species;
    tmt::Position pos;
    bool operator==(const Entity& rhs) const;
  };

  typedef std::map<std::string, int>   Chips;
  typedef std::map<std::string, Chips> Species;
  typedef std::vector<Entity>          Positions;

  Config();
  virtual ~Config();

  bool read(std::string const& configFile);
  bool read(std::istringstream& config);
  bool check() const;
  bool checkParamsDefined() const;
  bool checkSpeciesChipsCoherence() const;
  bool checkSpeciesAndBounds() const;
  bool checkInitialPositions() const;

  unsigned  getTics() const;
  void      setTics(unsigned t);
  unsigned  getWidth() const;
  void      setWidth(unsigned t);
  unsigned  getHeight() const;
  void      setHeight(unsigned t);
  Chips     getChips() const;
  void      setChips(const Chips &chps);
  Species   getSpecies() const;
  void      setSpecies(const Species &spcs);
  Positions getTermitePositions() const;
  void      setTermitePositions(const Positions &tpos);
  Positions getChipPositions() const;
  void      setChipPositions(const Positions &cpos);
  bool      getInitialized() const;
  void      setInitialized();

private:
  bool initialized;
  unsigned tics;
  unsigned width;
  unsigned height;
  // we store wood chips into a map, as wood species are supposely unique
  Chips chips;
  Species species;
  Positions termitePositions;
  Positions chipPositions;

  static const unsigned TMP_STR_MAX_LEN = 64;
  static const unsigned BUFFER_SIZE     = 1024;

  typedef std::string TmpString;
  typedef std::list<std::string> TmpList;
  typedef Species TmpHash;

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
  bool parserRun(std::istream& stream);
  void parserInit(ParserState &parser);
  void parserExecute(ParserState &parser, const char *data, int len);
  bool parserFinish(ParserState &parser);
  bool parserHasError(ParserState &parser);
  bool parserIsFinished(ParserState &parser);
};

#endif /* _CONFIG_H_ */
