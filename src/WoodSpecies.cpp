#include "WoodSpecies.hpp"

WoodSpecies::WoodSpecies(const std::string& nam) : name(nam) {}

WoodSpecies::~WoodSpecies() {}

std::string WoodSpecies::getName() const {return name;}
