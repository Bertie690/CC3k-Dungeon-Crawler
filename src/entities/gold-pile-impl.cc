module goldpile;

#ifdef __INTELLISENSE__
#include "gold-pile.cc"
#else
import itemtype;
#endif  // __INTELLISENSE__

GoldPile::GoldPile(Position position, GoldSize size) noexcept
    : Item{position, ItemType::GoldPile}, size{size} {}

void GoldPile::applyEffect(Character& character) {
  character.addGold(static_cast<int>(size));
}
