#include "ecs_world.h"
#include <iostream>

int main() {
  flecs::world world;
  std::cout << "start init world." << std::endl;
  ecs_world::InitWorld(world, TestParameters(3, 5));
  std::cout << "init world finish." << std::endl;

  world.system<EcsActor>().group_by<EcsRelationBelongsToCell>().write<GlobalCounter>().kind(flecs::OnUpdate).iter([](
      flecs::iter &it,
      EcsActor *actors) {
    auto group = it.world().entity(it.group_id());
    std::cout << " - group " << group.path() << ": table [" << it.table().str() << "]\n";
    auto *global_counter = it.world().get_mut<GlobalCounter>();

    for (auto i : it) {
      std::cout << "     {" << actors[i].guid << ", " << actors[i].is_active << "}\n";
      ++global_counter->calc_actors;
    }

    std::cout << "\n";
  });
  world.progress();
  std::cout << "global counter: " << world.get<GlobalCounter>()->calc_actors << std::endl;
  return 0;
}