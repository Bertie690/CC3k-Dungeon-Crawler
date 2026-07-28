module randomfloorgenerator;

#ifdef __INTELLISENSE__
#include <memory>
#include <set>
#include <vector>

#include "../entities/character.cc"
#include "../entities/gold-pile.cc"
#include "../entities/staircase.cc"
#include "../entities/stats.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../enums/room-type.cc"
#include "../enums/tile-type.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/factory-base.cc"
#include "../factories/floor-region.cc"
#include "../factories/gold-factory.cc"
#include "../factories/potion-factory.cc"
#include "../factories/staircase-factory.cc"
#include "cell.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "random-floor-generator.cc"
#include "room.cc"
#else
import <memory>;
import <set>;
import <vector>;
import character;
import dragonhoardfactory;
import enemyfactory;
import factorybase;
import floorregion;
import goldfactory;
import goldpile;
import goldsize;
import potionfactory;
import staircasefactory;
import stats;
import staircase;
import racetype;
import tiletype;
import roomtype;
import cell;
import floor;
import chamber;
import position;
import room;
#endif  // __INTELLISENSE__

using namespace std;

RandomFloorGenerator::RandomFloorGenerator(RNG& rng) : rng{rng}, factories{initializeFactories()} {}

vector<shared_ptr<FactoryBase>> RandomFloorGenerator::initializeFactories() {
  shared_ptr<EnemyFactory> enemyFactory = make_shared<EnemyFactory>(
      rng, make_shared<RandomSampleFloorPopulationStrategy>(rng, NUM_ENEMIES_SPAWNED));
  return {
      make_shared<StaircaseFactory>(rng),
      make_shared<PotionFactory>(
          rng, make_shared<RandomSampleFloorPopulationStrategy>(rng, NUM_POTIONS_SPAWNED)),
      make_shared<GoldFactory>(
          rng, make_shared<RandomSampleFloorPopulationStrategy>(rng, NUM_GOLD_PILES_SPAWNED)),
      make_shared<DragonHoardFactory>(rng, *enemyFactory,
                                      make_shared<SweepFloorPopulationStrategy>()),
      enemyFactory,
  };
}

Floor RandomFloorGenerator::generateFloor() {
  Floor floor = createBaseFloor(rng);

  // Build regions in one pass from the floor's chambers
  vector<FloorRegion> regions;
  for (Room* room : floor.getRooms()) {
    if (room->type() != RoomType::Chamber) {
      continue;
    }
    set<Position> chamberTiles;
    for (const Cell* cell : room->getCells()) {
      if (cell->tileType == TileType::Floor && !cell->isOccupied()) {
        chamberTiles.insert(cell->position);
      }
    }
    regions.push_back(FloorRegion{*room, move(chamberTiles)});
  }

  std::size_t playerRegion = rng.intRange(regions.size());
  floor.playerSpawn = selectRandomSpawn(regions[playerRegion], rng);

  // Staircase must be in a different chamber than the player

  // TODO: Do a quick dynamic_cast
  factories.front()->setPopulationStrategy(make_shared<FilterFloorPopulationStrategy>(
      make_shared<RandomSampleFloorPopulationStrategy>(rng, 1),
      [playerRoom = &regions[playerRegion].room](const FloorRegion& region) {
        return &region.room != playerRoom;
      }));

  for (const shared_ptr<FactoryBase>& factory : factories) {
    factory->execute(regions);
  }

  return floor;
}
