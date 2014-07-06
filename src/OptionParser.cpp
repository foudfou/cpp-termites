/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "OptionParser.hpp"
#include <getopt.h>
#include <iostream>
#include <random>
#include "log.h"
#include "config.h"

OptionParser::OptionParser(std::shared_ptr<Config> cnf): conf(cnf), debug(0) {}

OptionParser::~OptionParser() {}

bool OptionParser::parse(const int argc, char *const * argv)
{
  static struct option long_options[] = {
    /* These options set a flag. */
    {"debug", no_argument, &debug, 1},
    /* These options don't set a flag. We distinguish them by their indices. */
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'v'},
    {"height", required_argument, 0, 'H'},
    {"width", required_argument, 0, 'W'},
    {"termites", required_argument, 0, 't'},
    {"chips", required_argument, 0, 'c'}, // woodchips
    {"times", required_argument, 0, 'T'},
    {0, 0, 0, 0}                // convention
  };

  int c;
  while (1)
  {
    int option_index = 0;
    c = getopt_long(argc, argv, "hvH:W:t:c:T:",
                    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1) break;

    std::string opt = long_options[option_index].name;
    switch (c)
    {
    case 0:              // for options which set a flag
      if (opt == "debug")
        FILELog::ReportingLevel() = FILELog::FromString("DEBUG");
      break;

    case 'h':
      std::cout << usage;
      break;

    case 'v':
      std::cout << PACKAGE_NAME << " " << PACKAGE_VERSION << std::endl;
      std::cout << PACKAGE_COPYRIGHT << std::endl;
      break;

    case 'H':
      conf->setHeight(std::stoi(optarg));
      break;

    case 'W':
      conf->setWidth(std::stoi(optarg));
      break;

    case 't':
      std::cerr << "termites= " << optarg << std::endl;
      break;

    case 'c':
      std::cerr << "chips= " << optarg << std::endl;
      break;

    case 'T':
      conf->setTics(std::stoi(optarg));
      break;

    case '?':               // error
      /* getopt_long already printed an error message. */
      break;

    default:
      std::cout << "undefined " << c << std::endl;
    }
  }

  /* remaining command line arguments (not options). */
  if (optind < argc)
  {
    if (argc - optind > 1)
      FILE_LOG(logERROR) << "More than one configuration file given.";
    else
      configFile = argv[optind++];
  }

  // FIXME: TODO: check options complete

  return true;
}

void OptionParser::rand()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 6);

  for (int n=0; n<10; ++n)
    std::cout << dis(gen) << ' ';
  std::cout << '\n';
}
