cmake_minimum_required(VERSION 3.15)
project(2q_tests)
project(2Q_perfect)
project(2Q_cache)
project(2Q_interactive)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero" )
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  2q_tests
  src/2q_tests.cc
)
target_link_libraries(
  2q_tests
  GTest::gtest_main
)

add_executable(
  2Q_perfect
  src/cache_me_if_you_can.cpp
)

add_executable(
  2Q_interactive
  src/2q_interactive.cpp
)

add_executable(
  2Q_cache
  src/cache_me_if_you_can.cpp
)

target_compile_options(2Q_perfect PRIVATE "-DIS_PERFECT_=1" "-DDEBUG_")
target_compile_options(2Q_interactive PRIVATE "-DDEBUG_")
target_compile_options(2q_tests PRIVATE "-DDEBUG_")
target_compile_options(2Q_cache PRIVATE "-DIS_PERFECT_=0" "-DNDEBUG")



include(GoogleTest)
gtest_discover_tests(2q_tests)
