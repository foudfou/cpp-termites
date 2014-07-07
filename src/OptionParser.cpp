/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "OptionParser.hpp"
#include <getopt.h>
#include <iostream>
#include <sstream>
#include "log.h"
#include "config.h"
#include "helpers.hpp"

OptionParser::OptionParser(std::shared_ptr<Config> cnf):
  conf(cnf), logFile(nullptr) {}

OptionParser::~OptionParser() {}

bool OptionParser::parse(const int argc, char *const * argv)
{
  static struct option long_options[] = {
    {"debug", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"output", no_argument, NULL, 'o'},
    {"height", required_argument, NULL, 'H'},
    {"width", required_argument, NULL, 'W'},
    {"termites", required_argument, NULL, 't'},
    {"chips", required_argument, NULL, 'c'}, // woodchips
    {"times", required_argument, NULL, 'T'},
    {0, 0, 0, 0}                // convention
  };

  while (1)
  {
    int option_index = 0;
    int c = getopt_long(argc, argv, "+dhvo:H:W:t:c:T:",
                    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1) break;

    switch (c)
    {
    case 0:              // for options which set a flag
      break;
    case 'd':
      options["debug"] = "true";
      break;
    case 'h':
      std::cout << usage;
      return true;
      break;
    case 'v':
      std::cout << PACKAGE_NAME << " " << PACKAGE_VERSION << std::endl;
      std::cout << PACKAGE_COPYRIGHT << std::endl;
      return false;
      break;
    case 'o':
      options["logFileName"] = optarg;
      break;
    case 'H':
      options["height"] = optarg;
      break;
    case 'W':
      options["width"] = optarg;
      break;
    case 't':
      options["termiteAmount"] = optarg;
      break;
    case 'c':
      options["chipAmount"] = optarg;
      break;
    case 'T':
      options["tics"] = optarg;
      break;
    case '?':               // error
      return false;
      break;
    default:
      FILE_LOG(logERROR) << "Undefined option " << char(c);
    }
  }

  /* remaining command line arguments (not options). */
  if (optind < argc)
  {
    if (argc - optind > 1)
    {
      FILE_LOG(logERROR) << "More than one configuration file given.";
      return false;
    }
    else
    {
      options["configFileName"] = argv[optind++];
      FILE_LOG(logDEBUG) << "Configuration file set to "
                         << options["configFileName"];
    }
  }

  if (!check()) return false;
  if (!processInOrder()) return false;

  return true;
}

bool OptionParser::check()
{
  auto none = options.end();
  bool hasConf = options.find("configFileName") == none;
  bool hasAllOpts = options.find("height") == none &&
    options.find("width") == none && options.find("tics") == none &&
    options.find("termiteAmount") == none && options.find("chipAmount") == none;
  bool hasSomeOpts = options.find("height") == none ||
    options.find("width") == none || options.find("tics") == none ||
    options.find("termiteAmount") == none || options.find("chipAmount") == none;

  if (hasConf)
  {
    if (hasSomeOpts)
    {
      FILE_LOG(logERROR) << "Please provide either a configuration file "
        "OR all mandatory options (height, width, termites, chips, tics).";
      return false;
    }
  }
  else if (hasSomeOpts && !hasAllOpts)
  {
    FILE_LOG(logERROR) << "Please provide ALL options (height, width, "
      "termites, chips, tics).";
    return false;
  }
  return true;
}

bool OptionParser::processInOrder()
{
  if (!options["debug"].empty())
  {
    FILELog::ReportingLevel() = FILELog::FromString("DEBUG");
  }
  if (!options["logFileName"].empty())
  {
    setLogFile(options["logFileName"]);
  }
  if (!options["height"].empty())
  {
    conf->setHeight(std::stoi(options["height"]));
  }
  if (!options["width"].empty())
  {
    conf->setWidth(std::stoi(options["width"]));
  }
  if (!options["termiteAmount"].empty() &&  !options["chipAmount"].empty())
  {
    int height = conf->getHeight(), width = conf->getWidth();
    int tamount = std::stoi(options["termiteAmount"]);
    int camount = std::stoi(options["chipAmount"]);
    if (height * width < tamount + camount)
    {
      FILE_LOG(logERROR) << "Too many termites and chips for board size.";
      return false;
    }

    std::vector<int> randoms = tmt::pickn(tamount + camount, height * width);

    conf->setTermitePositions(buildEntities(tamount, randoms, width, 0));
    conf->setChipPositions(buildEntities(camount, randoms, width, tamount));
  }
  if (!options["tics"].empty())
  {
    conf->setTics(std::stoi(options["tics"]));
  }

  return true;
}

FILE* OptionParser::getLogFile() {
  return logFile;
}

void OptionParser::setLogFile(std::string filename)
{
  logFile = fopen(filename.c_str(), "a");
  Output2FILE::Stream() = logFile;
}

std::string OptionParser::getConfigFileName()
{
  return options["configFileName"];
}

std::vector<Config::Entity>
OptionParser::buildEntities(int amount, const std::vector<int> &randoms,
                            int width, int off) const
{
  std::vector<Config::Entity> positions;
  std::stringstream randomsStr;
  for (int i=0; i<amount; ++i) {
    auto pos = tmt::position(randoms[i+off], width);
    positions.push_back(Config::Entity({"species0", pos.first, pos.second}));
    if (FILELog::ReportingLevel() >= logDEBUG)
      randomsStr << '|' << pos.first << ':' << pos.second;
  }
  FILE_LOG(logDEBUG) << "entitiy positions: " << randomsStr.str();
  return positions;
}