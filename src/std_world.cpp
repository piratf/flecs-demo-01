#include "std_world.h"

namespace std_world {

void InitCells(World &world, const int64_t total_cells) {
  world.all_cells.reserve(total_cells);
  world.all_cells.clear();
  for (int i = 0; i < total_cells; ++i) {
    world.all_cells.emplace_back();
  }
}

void InitActors(World &world, const int64_t total_actors_in_each_cell) {
  world.all_actors.reserve(world.all_cells.size() * total_actors_in_each_cell);
  world.all_actors.clear();
  for (auto &cell : world.all_cells) {
    for (int i = 0; i < total_actors_in_each_cell; ++i) {
      world.all_actors.emplace_back();
      cell.members.emplace_back(i);
    }
  }
}

void InitWorld(World &world, const TestParameters& params) {
  world.counter = {};
  InitCells(world, params.total_cells);
  InitActors(world, params.total_actors_in_each_cell);
}

void LoopAllActors(World &world) {
  for (auto &cell : world.all_cells) {
    for (auto &index : cell.members) {
      auto &actor = world.all_actors[index];
      actor.is_active = !actor.is_active;
      ++world.counter.calc_actors;
    }
  }
}
}