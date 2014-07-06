/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "OptionParser.hpp"
#include <getopt.h>
#include <iostream>
#include "log.h"
#include "config.h"

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

  int c;
  while (1)
  {
    int option_index = 0;
    c = getopt_long(argc, argv, "+dhvo:H:W:t:c:T:",
                    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1) break;

    std::string opt = long_options[option_index].name;
    switch (c)
    {
    case 0:              // for options which set a flag
      break;
    case 'd':
      options["debug"] = "true";
      break;
    case 'h':
      std::cout << usage;
      return false;
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
  processInOrder();

  return true;
}

bool OptionParser::check()
{
  bool hasConf = !options["configFileName"].empty();
  bool hasAllOpts = !options["height"].empty() && !options["width"].empty() &&
    !options["tics"].empty() && !options["termiteAmount"].empty() &&
    !options["chipAmount"].empty();
  bool hasSomeOpts = !options["height"].empty() || !options["width"].empty() ||
    !options["tics"].empty() || !options["termiteAmount"].empty() ||
    !options["chipAmount"].empty();

  if (hasConf)
  {
    if (hasSomeOpts)
    {
      FILE_LOG(logERROR) << "Please provide either a configuration file OR options.";
      return false;
    }
  }
  else if (hasSomeOpts && !hasAllOpts)
  {
      FILE_LOG(logERROR) << "Please provide ALL options (height, width, termites, chips, tics).";
      return false;
  }
  return true;
}

void OptionParser::processInOrder()
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
  if (!options["termiteAmount"].empty())
  {
    // termiteAmount = std::stoi(optarg);
    // ... generate positions
    // conf->setTermitePositions()
  }
  if (!options["chipAmount"].empty())
  {
    // chipAmount = std::stoi(optarg);
    // ... generate positions
    // conf->setChipPositions()
  }
  if (!options["tics"].empty())
  {
    conf->setTics(std::stoi(options["tics"]));
  }
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
