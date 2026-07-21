export module character;

#pragma once

#ifdef __INTELLISENSE__
#include "character-concrete.cc"
#include "character-character.cc"
#include "character-move-strategy-type.cc"
#include "character-decorators.cc"
#include "character-character.cc"
#else
export import :concrete;
export import :character;
export import :movement;
export import :decorators;
export import :movestrategytype;
#endif  // __INTELLISENSE__
