module chamber;

#ifdef __INTELLISENSE__
#include <memory>
#include <utility>
#include <vector>

#include "chamber.cc"
#else
import <memory>;
import <utility>;
import <vector>;
#endif  // __INTELLISENSE__

using namespace std;

Chamber::Chamber(vector<unique_ptr<Cell>> cells) : Room{move(cells)} {}
RoomType Chamber::type() const { return RoomType::Chamber; }
