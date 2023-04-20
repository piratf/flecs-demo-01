OPTION(ENABLE_GPERF_TOOLS "switch gperftools option" OFF)

if (ENABLE_GPERF_TOOLS)
    message(STATUS "gperf switched on!")
    add_compile_options(-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
    add_compile_definitions(ENABLE_GPERF_TOOLS)

    add_library(tcmalloc_and_profiler SHARED IMPORTED)
    set_target_properties(tcmalloc_and_profiler PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtcmalloc_and_profiler.so)
    set_target_properties(tcmalloc_and_profiler PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/include)
    add_library(unwind SHARED IMPORTED)
    set_target_properties(unwind PROPERTIES IMPORTED_LOCATION /usr/local/lib/libunwind.so)
    set_target_properties(unwind PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/include)
endif()

macro(GPERF_LINK target)
    if (ENABLE_GPERF_TOOLS)
        target_link_libraries(${target}
                PRIVATE tcmalloc_and_profiler
                PRIVATE unwind)
    endif()
endmacro()