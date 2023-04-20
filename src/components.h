#pragma once

#include <array>
#include <flecs.h>

static const int kTotalActorNumberPerCell = 50;
struct EcsActor{
    std::array<char, 256> data_buffer = {};
    bool is_active = false;
};

static const int kTotalCellNumber = 20;
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