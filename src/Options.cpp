/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "Options.hpp"
#include <getopt.h>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include "config.h"
#include "ext/log.h"
#include "helpers.hpp"

Options::Options(): logFile(nullptr) {}

Options::~Options() {
  if (logFile) {
    std::fclose(logFile);
    logFile = nullptr;
  }
}

void Options::setConfig(std::shared_ptr<Config> cnf)
{
  conf = cnf;
}

bool Options::parse(const int argc, char* const* argv)
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

  optind = 1;                   // reset extern variable
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
      return true;
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
      tmt::log(logERROR, _("Undefined option '%c'."), c);
    }
  }

  /* remaining command line arguments (not options). */
  if (optind < argc)
  {
    if (argc - optind > 1)
    {
      tmt::log(logERROR, _("More than one configuration file given."));
      return false;
    }
    else
    {
      options["configFileName"] = argv[optind++];
      tmt::log(logDEBUG, _("Configuration file set to %s."),
               options["configFileName"].c_str());
    }
  }

  if (!check()) return false;
  if (conf->getInitialized())
  {
    tmt::log(logERROR, _("Config already initialized."));
    return false;
  }
  if (!processInOrder()) return false;
  if (initMode == InitMode::OPTS) conf->setInitialized();

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
      tmt::log(logERROR, _("Please provide either a configuration file "
        "or all mandatory options (height, width, termites, chips, tics)."));
      return false;
    }
    initMode = InitMode::FILE;
    return true;
  }
  else if (hasSomeOpts)
  {
    if (!hasAllOpts)
    {
      tmt::log(logERROR, _("Please provide all options (height, width, "
                           "termites, chips, tics)."));
      return false;
    }
    initMode = InitMode::OPTS;
    return true;
  }
  else {
    tmt::log(logERROR, _("Missing options or configuration file."));
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
      tmt::log(logERROR, _("Too many termites and chips for board size."));
      return false;
    }

    std::vector<int> randoms;
    try {
       randoms = tmt::pickn(tamount + camount, height * width);
    } catch (const std::invalid_argument& e) {
      tmt::log(logERROR, _("Invalid argument: %s."), e.what());
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
    tmt::log(logERROR, _("Cannot open log file: %s."), std::strerror(errno));
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
  tmt::log(logDEBUG, _("entitiy positions: %s."), randomsStr.str().c_str());
  return positions;
}
