This is a demo to test the performance of `flecs`.

The scenario is that we have `kTotalCellNumber` cells, and each cell contains `kTotalActorNumberPerCell` actors.
Different cells in the actual scene may calculate independently. Here we directly traversed all to observe performance overhead.

Target `ecs_benchmark` use `flecs` query to store the index information.
Target `std_benchmark` use a vector to store the index information.