export module defaultfloor;

#ifdef __INTELLISENSE__
#include "floordata.cc"
#else
import floordata;
#endif  // __INTELLISENSE__

export FloorData getDefaultFloorData();
