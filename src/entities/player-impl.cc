module player;

#ifdef __INTELLISENSE__
#include <variant>

#include "../enums/action.cc"
#include "../events/game-events.cc"
#include "../floor/floor.cc"
#include "player.cc"
#else
import <variant>;
#endif  // __INTELLISENSE__

// TODO @ShauryaSuri: Fix input handling
// Action Player::getNextMove(Floor&) {
//   Action action = nextAction;
//   nextAction = Pass{};
//   return action;
// }

// void Player::onNotify(const PlayerActionEvent& event) {
//   if (const Move* move = get_if<Move>(&event.action)) {
//     nextAction = *move;
//   } else if (const Attack* attack = get_if<Attack>(&event.action)) {
//     nextAction = *attack;
//   } else if (const UsePotion* potion = get_if<UsePotion>(&event.action)) {
//     nextAction = *potion;
//   } else {
//     nextAction = Pass{};
//   }
// }
