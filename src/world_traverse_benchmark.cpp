#include <benchmark/benchmark.h>
#include "ecs_world.h"
#include "std_world.h"
#include <memory>
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

class EcsWorldFixture : public benchmark::Fixture {
 public:
  std::unique_ptr<flecs::world> world;
  TestParameters test_parameters = {2, 5};

  ~EcsWorldFixture() {
#ifdef ENABLE_GPERF_TOOLS
    ProfilerStart("./ecs_world.quit.cpu.prof");
#endif
    if (world != nullptr) {
      ecs_world::CleanUpWorld(*world);
      world.reset();
    }
#ifdef ENABLE_GPERF_TOOLS
    ProfilerStop();
#endif
  }

  void SetUp(const ::benchmark::State &state) {
    TestParameters input(state.range(0), state.range(1));
    if (input != test_parameters) {
      test_parameters = input;
#ifdef ENABLE_GPERF_TOOLS
      ProfilerStart("./ecs_world.init.cpu.prof");
#endif
      world = std::make_unique<flecs::world>();
      ecs_world::InitWorld(*world, test_parameters);
#ifdef ENABLE_GPERF_TOOLS
      ProfilerStop();
#endif
    }
  }

  void TearDown(const ::benchmark::State &state) {
  }
};

BENCHMARK_DEFINE_F(EcsWorldFixture, EcsWorldProcess)(benchmark::State &state) {
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./ecs_world.update.cpu.prof");
#endif
  auto *counter = world->get_mut<GlobalCounter>();
  for (auto _ : state) {
    counter->calc_actors_per_loop = 0;
    world->progress();
  }
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif
  state.counters["Total Calc Actors"] = counter->total_calc_actors;
  state.counters["Per Loop"] = counter->calc_actors_per_loop;
//  printf("total calc actors: %lu\n", counter->total_calc_actors);
}

class StdWorldFixture : public benchmark::Fixture {
 public:
  std::unique_ptr<std_world::World> world;
  TestParameters test_parameters = {2, 5};

  ~StdWorldFixture() {
#ifdef ENABLE_GPERF_TOOLS
    ProfilerStart("./std_world.quit.cpu.prof");
#endif
    world.reset();
#ifdef ENABLE_GPERF_TOOLS
    ProfilerStop();
#endif
  }

  void SetUp(const ::benchmark::State &state) {
    TestParameters input(state.range(0), state.range(1));
    if (input != test_parameters) {
      test_parameters = input;
#ifdef ENABLE_GPERF_TOOLS
      // Calculate the destruct cost at exit
      DeferProfilerStoper profiler_stoper;
      ProfilerStart("./std_world.init.cpu.prof");
#endif
      world = std::make_unique<std_world::World>();
      std_world::InitWorld(*world, test_parameters);
#ifdef ENABLE_GPERF_TOOLS
      ProfilerStop();
#endif
    }
  }

  void TearDown(const ::benchmark::State &state) {
  }
};

BENCHMARK_DEFINE_F(StdWorldFixture, StdWorldProcess)(benchmark::State &state) {
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStart("./std_world.update.cpu.prof");
#endif
  for (auto _ : state) {
    world->counter.calc_actors_per_loop = 0;
    std_world::LoopAllActors(*world);
  }
#ifdef ENABLE_GPERF_TOOLS
  ProfilerStop();
#endif
  state.counters["Total Calc Actors"] = world->counter.total_calc_actors;
  state.counters["Per Loop"] = world->counter.calc_actors_per_loop;
//  printf("total calc actors: %lu, cells: %ld, actors in each cell: %ld\n", world.counter.total_calc_actors, state.range(0), state.range(1));
}

#ifdef ENABLE_GPERF_TOOLS
BENCHMARK_REGISTER_F(EcsWorldFixture, EcsWorldProcess)->Args({200, 5000});
BENCHMARK_REGISTER_F(StdWorldFixture, StdWorldProcess)->Args({200, 5000});
#else
// cell number growths
BENCHMARK_REGISTER_F(EcsWorldFixture, EcsWorldProcess)->Args({20, 500})->Args({200, 500})->Args({2000, 500});
BENCHMARK_REGISTER_F(StdWorldFixture, StdWorldProcess)->Args({20, 500})->Args({200, 500})->Args({2000, 500});
// actor number in each cell growths
BENCHMARK_REGISTER_F(EcsWorldFixture, EcsWorldProcess)->Args({200, 50})->Args({200, 500})->Args({200, 5000});
BENCHMARK_REGISTER_F(StdWorldFixture, StdWorldProcess)->Args({200, 50})->Args({200, 500})->Args({200, 5000});
#endif

BENCHMARK_MAIN();