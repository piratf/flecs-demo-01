#include <benchmark/benchmark.h>
#include "ecs_world.h"
#include "std_world.h"
#ifdef ENABLE_GPERF_TOOLS
#  include <gperftools/profiler.h>
class DeferProfilerStoper {
 public:
  DeferProfilerStoper() = default;
  ~DeferProfilerStoper() {
    ProfilerStop();
  }
};
#endif

static void EcsWorldProcess(benchmark::State &state) {
#ifdef ENABLE_GPERF_TOOLS
  // Calculate the destruct cost at exit
  DeferProfilerStoper profiler_stoper;
  ProfilerStart("./ecs_world.init.cpu.prof");
#endif
  flecs::world world;
  ecs_world::InitWorld(world, TestParameters(state.range(0), state.range(1)));
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./ecs_world.update.cpu.prof");
#endif
  for (auto _ : state) {
    world.progress();
  }
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./ecs_world.quit.cpu.prof");
#endif
  ecs_world::CleanUpWorld(world);
//  printf("total calc actors: %lu\n", world.get<GlobalCounter>()->calc_actors);
}

static void StdWorldProcess(benchmark::State &state) {
#ifdef ENABLE_GPERF_TOOLS
  // Calculate the destruct cost at exit
  DeferProfilerStoper profiler_stoper;
  ProfilerStart("./std_world.init.cpu.prof");
#endif
  std_world::World world;
  TestParameters test_parameters(state.range(0), state.range(1));
  std_world::InitWorld(world, test_parameters);
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif

#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./std_world.update.cpu.prof");
#endif
  for (auto _ : state) {
    std_world::LoopAllActors(world);
  }
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif

#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./std_world.quit.cpu.prof");
#endif
//  printf("total calc actors: %lu, cells: %ld, actors in each cell: %ld\n", world.counter.calc_actors, state.range(0), state.range(1));
}

// cell number growths
BENCHMARK(EcsWorldProcess)->Args({20, 500})->Args({200, 500})->Args({2000, 500});
BENCHMARK(StdWorldProcess)->Args({20, 500})->Args({200, 500})->Args({2000, 500});
// actor number in each cell growths
BENCHMARK(EcsWorldProcess)->Args({200, 50})->Args({200, 500})->Args({200, 5000});
BENCHMARK(StdWorldProcess)->Args({200, 50})->Args({200, 500})->Args({200, 5000});

BENCHMARK_MAIN();