cmake_minimum_required(VERSION 3.20)
project(libportablec C)

add_library(lp_core STATIC
  src/core/lp_arena.c
  src/core/lp_ring.c
  src/core/lp_fmt.c
  src/core/lp_crc32.c
  src/core/lp_bytes.c
)

target_include_directories(lp_core PUBLIC include)

# Choose one port library per build
add_library(lp_port STATIC port/posix/lp_port.c) # or baremetal
target_include_directories(lp_port PUBLIC include)

add_library(lp STATIC)
target_link_libraries(lp PUBLIC lp_core lp_port)

# Tests (host)
enable_testing()
add_executable(test_arena tests/test_arena.c)
target_link_libraries(test_arena PRIVATE lp)
add_test(NAME test_arena COMMAND test_arena)

