/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "Piece.hpp"

Piece::Piece() { }

Piece::~Piece() { }

bool Piece::isWoodChip() const {return false;}

bool Piece::isTermite() const {return false;}

std::string Piece::dump() const {return "Piece";}
