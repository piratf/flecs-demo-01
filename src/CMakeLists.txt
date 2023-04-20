add_library(world_dependency
        components.h
        components.cpp
        )
target_link_libraries(world_dependency
        PUBLIC ${FLECS_LIB_NAME}
        )
add_gtest_benchmark_default(ecs_benchmark)
target_link_libraries(ecs_benchmark
        world_dependency)
add_gtest_benchmark_default(std_benchmark)
target_link_libraries(std_benchmark
        world_dependency)