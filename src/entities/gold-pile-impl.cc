module goldpile;

#ifdef __INTELLISENSE__
#include "gold-pile.cc"
#include "player.cc"
#else
import itemtype;
import player;
#endif  // __INTELLISENSE__

GoldPile::GoldPile(Position position, GoldSize size) noexcept
    : Item{position, ItemType::GoldPile}, size{size} {}

void GoldPile::applyEffect(Character& character) {
  Player& player = dynamic_cast<Player&>(character);
  player.gold += static_cast<int>(size);
}
