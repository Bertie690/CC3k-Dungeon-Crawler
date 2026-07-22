module goldfactory;

#ifdef __INTELLISENSE__
#include <memory>

#include "gold-factory.cc"
#else
import <memory>;
#endif  // __INTELLISENSE__

GoldSize GoldFactory::randomGoldSize() {
  int randomGoldSizeSelection = rng.intRange(8);
  if (randomGoldSizeSelection < 5) {
    return GoldSize::Normal;
  } else if (randomGoldSizeSelection < 7) {
    return GoldSize::Small;
  } else {
    return GoldSize::DragonHoard; // randomGoldSizeSelection == 7
  }
}

std::shared_ptr<GoldPile> GoldFactory::create(const Position& position) {
  return create(position, randomGoldSize()); 
}
std::shared_ptr<GoldPile> GoldFactory::create(const Position& position, GoldSize size) {
  return std::make_shared<GoldPile>(position, size);
}
