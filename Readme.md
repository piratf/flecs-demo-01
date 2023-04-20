This is a demo to test the performance of `flecs`.

The scenario is that we have a world with lots of cells, and each cell contains lots of actors inside.

Different cells in the actual scene may calculate independently. Here we directly traversed all to observe performance overhead.

- Target `ecs_world` use `flecs` query to store the index information.
- Target `std_world` use a vector to store the index information.

## Quick Start
All third party dependency will automatically download and build by cmake.

Get benchmark result
```bash
# generate in release mode
/usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" -S /tmp/flecs-demo-01 -B /tmp/flecs-demo-01/cmake-build-release
# build in release mode
/usr/bin/cmake --build /tmp/flecs-demo-01/cmake-build-release --target world_traverse_benchmark -- -j 8
# run benchmark
cd /tmp/flecs-demo-01/cmake-build-release/src/
./world_traverse_benchmark
```

Or get profile outputs
```bash
# generate release mode with debug info and open gperf switch, notice the "-DENABLE_GPERF_TOOLS=ON"
/usr/bin/cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DENABLE_GPERF_TOOLS=ON -G "CodeBlocks - Unix Makefiles" -S /tmp/flecs-demo-01 -B /tmp/flecs-demo-01/cmake-build-relwithdebinfo
# build
/usr/bin/cmake --build /tmp/flecs-demo-01/cmake-build-relwithdebinfo --target world_traverse_benchmark -- -j 8
# run and get cpu profile from gperf
cd /tmp/flecs-demo-01/cmake-build-relwithdebinfo/src/
./world_traverse_benchmark
# cpu profile output in the same path here
# you may want to adjust the parameters in `world_traverse_benchmark.cpp`.
# use pprof to get the profile output
go install github.com/google/pprof@latest
$(go env GOBIN)/pprof -pdf -output ecs_world.update.pdf world_traverse_benchmark ecs_world.update.cpu.prof
```