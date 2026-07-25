module potionfactory;

#ifdef __INTELLISENSE__
#include "potion-factory.cc"
#else
import <memory>;
#endif

std::shared_ptr<Potion> PotionFactory::create(const Position& position) {
  return create(position, static_cast<PotionType>(rng.intRange(6)));
}
std::shared_ptr<Potion> PotionFactory::create(const Position& position, PotionType type) {
  return std::make_shared<Potion>(position, type);
}
