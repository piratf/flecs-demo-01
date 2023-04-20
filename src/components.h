#pragma once

#include <array>
#include <flecs.h>

struct EcsActor{
    std::array<char, 256> data_buffer = {};
    bool is_active = false;
};

struct EcsCell {
    flecs::query<EcsActor> actors_query;
};
struct EcsRelationBelongsToCell{};
struct EcsWorldCells {
    ecs_map_t cells;
};

struct GlobalCounter {
    uint64_t calc_actors = 0;
};

struct TestParameters {
  int64_t total_cells = 0;
  int64_t total_actors_in_each_cell = 0;
  TestParameters(int64_t total_cells, int64_t total_actors_in_each_cell);
};