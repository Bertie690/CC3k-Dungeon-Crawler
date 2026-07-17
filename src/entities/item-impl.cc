module item;

#ifdef __INTELLISENSE__
#include "item.cc"
#endif  // __INTELLISENSE__

Item::Item(Position position, ItemType type) noexcept : Entity{position}, type(type) {}
