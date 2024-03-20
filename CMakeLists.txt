cmake_minimum_required(VERSION 3.14)
project(2q_tests)
project(2Q_perfect)
project(2Q_cache)
project(2Q_interactive)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero")
set(CMAKE_BUILD_FILES_DIRECTORY ${./build})
SET(EXECUTABLE_OUTPUT_PATH ${./build})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  2q_tests
  2q_tests.cc
)
target_link_libraries(
  2q_tests
  GTest::gtest_main
)

add_executable(
  2Q_perfect
  cache_me_if_you_can.cpp
)

add_executable(
  2Q_interactive
  2q_interactive.cpp
)

target_compile_options(2Q_perfect PRIVATE "-DIS_PERFECT_=1" "-DDEBUG_" "-o 2Q_perfect")
target_compile_options(2Q_interactive PRIVATE "-o 2Q_interactive" "-DDEBUG_")
target_compile_options(2q_tests PRIVATE "-DDEBUG_")

add_executable(
  2Q_cache
  cache_me_if_you_can.cpp
)

target_compile_options(2Q_cache PRIVATE "-DIS_PERFECT_=0" "-DNDEBUG")
target_link_options(2Q_cache PRIVATE "-o 2Q_cache")

include(GoogleTest)
gtest_discover_tests(2q_tests)
