module randomfloorgenerator;

#ifdef __INTELLISENSE__
#include "random-floor-generator.cc"
#endif  // __INTELLISENSE__

RandomFloorGenerator::RandomFloorGenerator(RNG& rng) : rng{rng} {}

Floor RandomFloorGenerator::generateFloor() {
  Floor floor = createBaseFloor(rng);

  // TODO: randomly place @, enemies, potions, gold, stairs
  return floor;
}
