#pragma once
#include <vector>
#include "components.h"

namespace std_world {
struct StdCell {
  std::vector<size_t> members;
};

struct World {
  GlobalCounter counter;
  std::vector<StdCell> all_cells;
  std::vector<EcsActor> all_actors;
};

void InitWorld(World &world, const TestParameters& params);
void LoopAllActors(World &world);
}