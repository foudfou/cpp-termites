/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "Options.hpp"
#include <getopt.h>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include "ext/log.h"
#include "config.h"
#include "helpers.hpp"

Options::Options(): logFile(nullptr) {}

Options::~Options() {
  if (logFile) {
    std::fclose(logFile);
    logFile = nullptr;
  }
}

bool Options::setConfig(std::shared_ptr<Config> cnf)
{
  if (cnf->getInitialized())
  {
    FILE_LOG(logERROR) << "Config already initialized.";
    return false;
  }
  conf = cnf;
  return true;
}

bool Options::parse(const int argc, char *const * argv)
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

  conf->setInitialized();
  return true;
}

bool Options::check()
{
  bool hasConf = options.count("configFileName");
  bool hasAllOpts = options.count("height") && options.count("width") &&
    options.count("tics") && options.count("termiteAmount") &&
    options.count("chipAmount");
  bool hasSomeOpts = options.count("height") || options.count("width") ||
    options.count("tics") || options.count("termiteAmount") ||
    options.count("chipAmount");

  if (hasConf)
  {
    if (hasSomeOpts)
    {
      FILE_LOG(logERROR) << "Please provide either a configuration file "
        "OR all mandatory options (height, width, termites, chips, tics).";
      return false;
    }
    return true;
  }
  else if (hasSomeOpts)
  {
    if (!hasAllOpts)
    {
      FILE_LOG(logERROR) << "Please provide ALL options (height, width, "
        "termites, chips, tics).";
      return false;
    }
    return true;
  }
  else {
    FILE_LOG(logERROR) << "Missing options or configuration file.";
    return false;
  }
}

bool Options::processInOrder()
{
  if (options.count("debug"))
  {
    FILELog::ReportingLevel() = FILELog::FromString("DEBUG");
  }
  if (options.count("logFileName"))
  {
    if (!setLogFile(options["logFileName"])) return false;
  }
  if (options.count("height"))
  {
    conf->setHeight(std::stoi(options["height"]));
  }
  if (options.count("width"))
  {
    conf->setWidth(std::stoi(options["width"]));
  }
  if (options.count("termiteAmount") && options.count("chipAmount"))
  {
    int height = conf->getHeight(), width = conf->getWidth();
    int tamount = std::stoi(options["termiteAmount"]);
    int camount = std::stoi(options["chipAmount"]);
    if (height * width < tamount + camount)
    {
      FILE_LOG(logERROR) << "Too many termites and chips for board size.";
      return false;
    }

    std::vector<int> randoms;
    try {
       randoms = tmt::pickn(tamount + camount, height * width);
    } catch (const std::invalid_argument& e) {
      FILE_LOG(logERROR) << "Invalid argument: " << e.what();
      return false;
    }

    conf->setTermitePositions(buildEntities(tamount, randoms, width, 0));
    conf->setChipPositions(buildEntities(camount, randoms, width, tamount));
  }
  if (options.count("tics"))
  {
    conf->setTics(std::stoi(options["tics"]));
  }

  return true;
}

bool Options::setLogFile(std::string filename)
{
  logFile = std::fopen(filename.c_str(), "a");
  if (logFile == NULL)
  {
    FILE_LOG(logERROR) << "Cannot open log file: " << std::strerror(errno);
    return false;
  }
  Output2FILE::Stream() = logFile;
  return true;
}

std::string Options::getConfigFileName()
{
  return options["configFileName"];
}

std::vector<Config::Entity>
Options::buildEntities(int amount, const std::vector<int> &randoms,
                            int width, int off) const
{
  std::vector<Config::Entity> positions;
  std::stringstream randomsStr;
  for (int i=0; i<amount; ++i) {
    auto pos = tmt::rankToCoord2D(randoms[i+off], width);
    positions.push_back(Config::Entity({"species0", pos.first, pos.second}));
    if (FILELog::ReportingLevel() >= logDEBUG)
      randomsStr << '|' << pos.first << ':' << pos.second;
  }
  FILE_LOG(logDEBUG) << "entitiy positions: " << randomsStr.str();
  return positions;
}
