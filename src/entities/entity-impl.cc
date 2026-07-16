module entity;

#ifdef __INTELLISENSE__
#include "entity.cc"
#endif  // __INTELLISENSE__

Entity::Entity(const Position& position) noexcept : position(position) {}
Entity::~Entity() = default;
bool Entity::hasCollision() const { return true; }