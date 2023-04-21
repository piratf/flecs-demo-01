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
        .set<EcsCell>(EcsCell(i));
  }
}

void InitActors(const flecs::world &world, const int64_t total_actors_in_each_cell) {
  world.component<EcsActor>();
  auto all_cells_q = world.query<const EcsCell>();
  all_cells_q.each([&world, total_actors_in_each_cell](flecs::entity cell_entity, const EcsCell &cell) {
    for (int i = 0; i < total_actors_in_each_cell; ++i) {
      world.entity().set<EcsActor>(EcsActor(cell.cell_id * 10000 + i)).add<EcsRelationBelongsToCell, flecs::entity>(
          cell_entity);
    }
  });
  all_cells_q.destruct();
}

void InitWorld(const flecs::world &world, const TestParameters &parameters) {
  world.set<GlobalCounter>({});
  InitCells(world, parameters.total_cells);
  InitActors(world, parameters.total_actors_in_each_cell);
  world.system<EcsActor>().group_by<EcsRelationBelongsToCell>().kind(flecs::OnUpdate).iter([](flecs::iter& it, EcsActor *actors) {
    for (auto i : it) {
      actors[i].is_active = !actors[i].is_active;
    }
  });
}

void CleanUpWorld(const flecs::world &world) {
  auto *cells = world.get_mut<EcsWorldCells>();
  ecs_map_fini(&cells->cells);
}
}