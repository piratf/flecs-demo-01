add_library(world_dependency
        components.h
        components.cpp
        )
target_link_libraries(world_dependency
        PUBLIC ${FLECS_LIB_NAME}
        )

add_library(ecs_world
        ecs_world.cpp
        ecs_world.h
        )
target_link_libraries(ecs_world
        world_dependency)

add_library(std_world
        std_world.cpp
        std_world.h
        )
target_link_libraries(std_world
        world_dependency)

add_gtest_benchmark_default(world_traverse_benchmark)
target_link_libraries(world_traverse_benchmark
        PRIVATE ecs_world
        PRIVATE std_world)
GPERF_LINK(world_traverse_benchmark)