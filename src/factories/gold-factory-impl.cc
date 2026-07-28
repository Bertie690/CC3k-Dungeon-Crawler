module goldfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <set>
#include <stdexcept>

#include "gold-factory.cc"
#else
import <memory>;
import <set>;
import <stdexcept>;
#endif  // __INTELLISENSE__

std::tuple<GoldSize> GoldFactory::generateArgs() {
  int randomGoldSizeSelection = rng.intRange(8);
  if (randomGoldSizeSelection < 5) {
    return std::make_tuple(GoldSize::Normal);
  } else if (randomGoldSizeSelection < 7) {
    return std::make_tuple(GoldSize::Small);
  } else {
    return std::make_tuple(GoldSize::DragonHoard);  // randomGoldSizeSelection == 7
  }
}

std::shared_ptr<GoldPile> GoldFactory::create(const Position& position, GoldSize size) const {
  return std::make_shared<GoldPile>(position, size);
}
