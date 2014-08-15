#ifndef CONFIGFIXT_H
#define CONFIGFIXT_H

#include "Config.hpp"

inline Config confFixt1()
{
  Config conf;
  conf.setTics(40);
  conf.setWidth(7);
  conf.setHeight(8);
  conf.setChips({ {"pine",0}, {"balsa",1}, {"oak",2}, {"ebony",1},
    {"cypress",1} });
  conf.setSpecies({
    {"red",   { {"pine",0}, {"balsa",1}, {"oak",2} } },
    {"brown", { {"balsa",0}, {"ebony",1} } },
    {"green", { {"cypress",0} } },
  });
  conf.setTermitePositions({ {"red",{5,4}}, {"brown",{1,1}}, });
  conf.setChipPositions({ {"pine",{2,2}}, {"pine",{5,5}}, {"balsa",{6,6}},
    {"balsa",{3,2}}, {"oak",{6,7}} });
  return conf;
}

#endif /* CONFIGFIXT_H */
