#pragma once

#include <flecs.h>
#include "components.h"

namespace ecs_world {
void InitWorld(const flecs::world &world, const TestParameters& parameters);
void LoopAllActors(const EcsCell &cell, GlobalCounter &counter);
void CleanUpWorld(const flecs::world &world);
}