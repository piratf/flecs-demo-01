This is a demo to test the performance of `flecs`.

The scenario is that we have a world with lots of cells, and each cell contains lots of actors inside.
Different cells in the actual scene may calculate independently. Here we directly traversed all to observe performance overhead.

Target `ecs_world` use `flecs` query to store the index information.
Target `std_world` use a vector to store the index information.