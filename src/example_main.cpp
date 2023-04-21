#include "ecs_world.h"
#include <iostream>

void InitSystemWithSingletonComponent(flecs::world & world) {
  world.system<EcsActor, GlobalCounter>().term_at(2).singleton().group_by<EcsRelationBelongsToCell>().write<
      GlobalCounter>().kind(flecs::OnUpdate).iter([](
      flecs::iter &it,
      EcsActor *actors, GlobalCounter* global_counter) {
    auto group = it.world().entity(it.group_id());
    std::cout << " - group " << group.path() << ": table [" << it.table().str() << "]\n";

    for (auto i : it) {
      std::cout << "     {" << actors[i].guid << ", " << actors[i].is_active << "}\n";
      ++global_counter->calc_actors;
    }

    std::cout << "\n";
  });
}

void GetMutSingletonInsideSystem(flecs::world & world) {
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
}

int main() {
  flecs::world world;
  std::cout << "start init world." << std::endl;
  ecs_world::InitWorld(world, TestParameters(2, 3));
  std::cout << "init world finish." << std::endl;

//  InitSystemWithSingletonComponent(world);
  // outputs:
  // - group ::542: table [EcsActor, (EcsRelationBelongsToCell,542)]
  //     {0, 0}
  //
  // - group ::542: table [EcsActor, (EcsRelationBelongsToCell,542)]
  //     {1, 0}
  //
  // - group ::542: table [EcsActor, (EcsRelationBelongsToCell,542)]
  //     {2, 0}
  //
  // - group ::543: table [EcsActor, (EcsRelationBelongsToCell,543)]
  //     {10000, 0}
  //
  // - group ::543: table [EcsActor, (EcsRelationBelongsToCell,543)]
  //     {10001, 0}
  //
  // - group ::543: table [EcsActor, (EcsRelationBelongsToCell,543)]
  //     {10002, 0}
  //
  //global counter: 6

  GetMutSingletonInsideSystem(world);
  // outputs:
  // - group ::542: table [EcsActor, (EcsRelationBelongsToCell,542)]
  //     {0, 0}
  //     {1, 0}
  //     {2, 0}
  //
  // - group ::543: table [EcsActor, (EcsRelationBelongsToCell,543)]
  //     {10000, 0}
  //     {10001, 0}
  //     {10002, 0}
  //
  //global counter: 6

  world.progress();
  std::cout << "global counter: " << world.get<GlobalCounter>()->calc_actors << std::endl;
  return 0;
}