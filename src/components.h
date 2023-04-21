#pragma once

#include <array>
#include <flecs.h>

struct EcsActor {
  uint64_t guid = 0;
  std::array<char, 256> data_buffer = {};
  bool is_active = false;
  EcsActor() = default;
  EcsActor(uint64_t guid) : guid(guid) {}
};

struct EcsCell {
  int cell_id = 0;
  EcsCell() = default;
  explicit EcsCell(int cell_id) : cell_id(cell_id) {}
};
struct EcsRelationBelongsToCell {};
struct EcsWorldCells {
  ecs_map_t cells;
};
struct EcsGroup1 {};
struct EcsGroup2 {};

struct GlobalCounter {
  uint64_t calc_actors = 0;
};

struct TestParameters {
  int64_t total_cells = 0;
  int64_t total_actors_in_each_cell = 0;
  TestParameters(int64_t total_cells, int64_t total_actors_in_each_cell);
};