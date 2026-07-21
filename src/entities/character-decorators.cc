export module character:decorators;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include "character-character.cc"
#include "../floor/position.cc"
#else
import <memory>;
import position;
import :character;
#endif // __INTELLISENSE__

class CharacterDecorator : public Character {
  std::unique_ptr<Character> character;

  // TODO: add method forwards
 public:
  // this duplicates the position of the underlying character, but the alternative is inheriting from a base class twice...
  CharacterDecorator(std::unique_ptr<Character> character)
      : character(std::move(character)) {}

  virtual ~CharacterDecorator() = default;
};