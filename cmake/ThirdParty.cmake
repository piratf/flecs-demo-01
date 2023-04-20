include(FetchContent)
FetchContent_Declare(
        flecs
        URL https://github.com/SanderMertens/flecs/archive/refs/tags/v3.2.0.tar.gz
)
set(FLECS_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(FLECS_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(flecs)
SET(FLECS_LIB_NAME  "flecs_static" CACHE INTERNAL "FLECS_LIB_NAME")
target_compile_options(flecs_static PRIVATE -w)
target_compile_options(flecs PRIVATE -w)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.tar.gz
        CMAKE_CACHE_DEFAULT_ARGS -DGOOGLETEST_PATH=${FETCHCONTENT_BASE_DIR}/googletest-src
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

MACRO(add_gtest_benchmark target_name)
    add_executable(${target_name} "${target_name}.cpp")
    target_link_libraries(${target_name}
            benchmark
            ${CMAKE_THREAD_LIBS_INIT}
            gtest_main)
ENDMACRO()

MACRO(add_gtest_benchmark_default target_name)
    add_gtest_benchmark(${target_name})

    add_test(NAME ${target_name} COMMAND ${target_name} --benchmark_min_time=0.01)
ENDMACRO()