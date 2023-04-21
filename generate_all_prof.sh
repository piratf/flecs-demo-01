#!/bin/bash

cd cmake-build-relwithdebinfo/src
$(go env GOBIN)/pprof -pdf -output ecs_world.init.pdf world_traverse_benchmark ecs_world.init.cpu.prof
$(go env GOBIN)/pprof -pdf -output ecs_world.update.pdf world_traverse_benchmark ecs_world.update.cpu.prof
$(go env GOBIN)/pprof -pdf -output ecs_world.quit.pdf world_traverse_benchmark ecs_world.quit.cpu.prof
$(go env GOBIN)/pprof -pdf -output std_world.init.pdf world_traverse_benchmark std_world.init.cpu.prof
$(go env GOBIN)/pprof -pdf -output std_world.update.pdf world_traverse_benchmark std_world.update.cpu.prof
$(go env GOBIN)/pprof -pdf -output std_world.quit.pdf world_traverse_benchmark std_world.quit.cpu.prof