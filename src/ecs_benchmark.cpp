#include <benchmark/benchmark.h>
#include <flecs.h>
#include "components.h"

void InitCells(const flecs::world& world) {
    world.component<EcsRelationBelongsToCell>();
    world.component<EcsCell>();
    auto *cells = world.get_mut<EcsWorldCells>();
    ecs_map_init(&cells->cells, nullptr);
    for (int i = 0; i < kTotalCellNumber; i++) {
        auto *p_cell = ecs_map_ensure(&cells->cells, i);
        *p_cell = world.entity()
                .set<EcsCell>({});
    }
}

void InitActors(const flecs::world& world) {
    world.component<EcsActor>();
    auto all_cells_q = world.query_builder<EcsCell>().build();
    // save query to cell, because creating query is expansive
    all_cells_q.each([&world](flecs::entity cell_entity, EcsCell &cell) {
        for (int i = 0; i < kTotalActorNumberPerCell; ++i) {
            world.entity().set<EcsActor>({}).add<EcsRelationBelongsToCell>(cell_entity);
        }
        cell.actors_query = world.query_builder<EcsActor>().with<EcsRelationBelongsToCell>(cell_entity).build();
    });
    all_cells_q.destruct();
}

void InitWorld(const flecs::world& world) {
    world.set<GlobalCounter>({});
    InitCells(world);
    InitActors(world);
}

void LoopAllActors(const flecs::world& world) {
    auto all_cells_q = world.query_builder<const EcsCell, GlobalCounter>().term_at(2).singleton().build();
    all_cells_q.each([&world](flecs::entity cell_entity, const EcsCell &cell, GlobalCounter& counter) {
        cell.actors_query.each( [&counter](EcsActor &actor) {
            actor.is_active = !actor.is_active;
            ++counter.calc_actors;
        });
    });
    all_cells_q.destruct();
}

void CleanUpWorld(const flecs::world& world) {
    auto all_cells_q = world.query_builder<EcsCell>().build();
    all_cells_q.each([&world](flecs::entity cell_entity, EcsCell &cell) {
        cell.actors_query.destruct();
    });
    all_cells_q.destruct();
}

static void EcsWorldProcess(benchmark::State& state) {
    flecs::world world;
    InitWorld(world);
    for (auto _ : state) {
        LoopAllActors(world);
    }
    CleanUpWorld(world);
    printf("total calc actors: %lu\n", world.get<GlobalCounter>()->calc_actors);
}

BENCHMARK(EcsWorldProcess);

BENCHMARK_MAIN();