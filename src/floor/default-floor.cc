export module defaultfloor;

#ifdef __INTELLISENSE__
#include "floor-data.cc"
#else
import floordata;
#endif  // __INTELLISENSE__

export FloorData getDefaultFloorData();
