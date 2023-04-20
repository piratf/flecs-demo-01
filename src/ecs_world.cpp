#include "ecs_world.h"

namespace ecs_world {
void InitCells(const flecs::world &world, const int64_t total_cells) {
  world.component<EcsRelationBelongsToCell>();
  world.component<EcsCell>();
  auto *cells = world.get_mut<EcsWorldCells>();
  ecs_map_init(&cells->cells, nullptr);
  for (int i = 0; i < total_cells; i++) {
    auto *p_cell = ecs_map_ensure(&cells->cells, i);
    *p_cell = world.entity()
        .set<EcsCell>({});
  }
}

void InitActors(const flecs::world &world, const int64_t total_actors_in_each_cell) {
  world.component<EcsActor>();
  auto all_cells_q = world.query_builder<EcsCell>().build();
  // save query to cell, because creating query is expansive
  all_cells_q.each([&world, total_actors_in_each_cell](flecs::entity cell_entity, EcsCell &cell) {
    for (int i = 0; i < total_actors_in_each_cell; ++i) {
      world.entity().set<EcsActor>({}).add<EcsRelationBelongsToCell>(cell_entity);
    }
    cell.actors_query = world.query_builder<EcsActor>().with<EcsRelationBelongsToCell>(cell_entity).build();
  });
  all_cells_q.destruct();
}

void InitWorld(const flecs::world &world, const TestParameters& parameters) {
  world.set<GlobalCounter>({});
  InitCells(world, parameters.total_cells);
  InitActors(world, parameters.total_actors_in_each_cell);
  world.system<const EcsCell, GlobalCounter>().term_at(2).singleton().each(ecs_world::LoopAllActors);
}

void LoopAllActors(const EcsCell &cell, GlobalCounter &counter) {
  cell.actors_query.each([&counter](EcsActor &actor) {
    actor.is_active = !actor.is_active;
    ++counter.calc_actors;
  });
}

void CleanUpWorld(const flecs::world &world) {
  auto all_cells_q = world.query_builder<EcsCell>().build();
  all_cells_q.each([&world](flecs::entity cell_entity, EcsCell &cell) {
    cell.actors_query.destruct();
  });
  all_cells_q.destruct();

  auto *cells = world.get_mut<EcsWorldCells>();
  ecs_map_fini(&cells->cells);
}
}