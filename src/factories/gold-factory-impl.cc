module goldfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "gold-factory.cc"
#else
import <memory>;
import <vector>;
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

void GoldFactory::process(Chamber& chamber, std::vector<Position>& availablePositions, GoldSize size) {
  int selectedIndex = rng.intRange(static_cast<int>(availablePositions.size()));
  Position position = availablePositions[selectedIndex];
  availablePositions[selectedIndex] = availablePositions.back();
  availablePositions.pop_back();
  chamber[position].add(create(position, size));
}
