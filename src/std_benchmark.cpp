#include <unordered_map>
#include <benchmark/benchmark.h>
#include "components.h"

struct StdCell {
    std::vector<size_t> members;
};

struct World {
    GlobalCounter counter;
    std::vector<StdCell> all_cells;
    std::vector<EcsActor> all_actors;
};

void InitCells(World &world) {
    world.all_cells.reserve(kTotalCellNumber);
    for (int i = 0; i < kTotalCellNumber; ++i) {
        world.all_cells.emplace_back();
    }
}

void InitActors(World &world) {
    world.all_actors.reserve(kTotalCellNumber * kTotalActorNumberPerCell);
    for (auto &cell: world.all_cells) {
        for (int i = 0; i < kTotalActorNumberPerCell; ++i) {
            world.all_actors.emplace_back();
            cell.members.emplace_back(i);
        }
    }
}

void InitWorld(World &world) {
    InitCells(world);
    InitActors(world);
}

void LoopAllActors(World& world) {
    for (auto &cell: world.all_cells) {
        for (auto& index: cell.members) {
            auto& actor = world.all_actors[index];
            actor.is_active = !actor.is_active;
            ++world.counter.calc_actors;
        }
    }
}

static void EcsWorldProcess(benchmark::State &state) {
    World world;
    InitWorld(world);
    for (auto _: state) {
        LoopAllActors(world);
    }
    printf("total calc actors: %lu\n", world.counter.calc_actors);
}

BENCHMARK(EcsWorldProcess);

BENCHMARK_MAIN();