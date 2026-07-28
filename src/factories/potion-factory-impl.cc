module potionfactory;

#ifdef __INTELLISENSE__
#include "potion-factory.cc"
#endif  // __INTELLISENSE__

std::tuple<PotionType> PotionFactory::generateArgs() {
  return std::make_tuple(static_cast<PotionType>(rng.intRange(6)));
}

std::shared_ptr<Potion> PotionFactory::create(const Position& position, PotionType type) const {
  return std::make_shared<Potion>(position, type);
}
