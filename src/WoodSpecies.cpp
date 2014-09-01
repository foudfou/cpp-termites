/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "WoodSpecies.hpp"

WoodSpecies::WoodSpecies(const std::string& nam) : name(nam) {}

WoodSpecies::~WoodSpecies() {}

std::string WoodSpecies::getName() const {return name;}
