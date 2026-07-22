export module character;

#pragma once

#ifdef __INTELLISENSE__
#include "character-base.cc"
#include "character-move-strategy-type.cc"
#include "character-decorators.cc"
#include "character-character.cc"
#else
export import :base;
export import :character;
export import :movement;
export import :decorators;
export import :movestrategytype;
#endif  // __INTELLISENSE__
