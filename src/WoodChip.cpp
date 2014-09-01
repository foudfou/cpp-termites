/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "WoodChip.hpp"

WoodChip::WoodChip(const WoodSpeciesPtr& spc) : species(spc) {}

WoodChip::~WoodChip() {}

bool WoodChip::isWoodChip() const {return true;}

std::string WoodChip::dump() const {return "WoodChip";}

WoodSpeciesPtr WoodChip::getSpecies() const {return species;}
