#include "ecs_world.h"
#include <iostream>

uint64_t group_by_relation(flecs::world_t *ecs, flecs::table_t *table,
                           flecs::entity_t id, void *) {
  // Use ecs_search to find the target for the relationship in the table
  flecs::id_t match;
  if (ecs_search(ecs, table, flecs::id(id, flecs::Wildcard), &match) != -1) {
    return flecs::id(ecs, match).second(); // World cell is 2nd element of pair
  }
  return 0;
}

int main() {
  flecs::world world;
  TestParameters parameters(3, 5);
  std::cout << "begin init world." << std::endl;
  ecs_world::InitWorld(world, parameters);
  std::cout << "init world finish." << std::endl;

  auto q = world.query_builder<EcsActor>()
      .group_by<EcsRelationBelongsToCell>()
      .build();

  q.iter([&](flecs::iter& it, EcsActor *actors) {
    auto group = world.entity(it.group_id());
    std::cout << " - group " << group.path() << ": table ["
              << it.table().str() << "]\n";

    for (auto i : it) {
      std::cout << "     {" << actors[i].guid << ", " << actors[i].is_active << "}\n";
    }

    std::cout << "\n";
  });
  return 0;
}