cmake_minimum_required(VERSION 3.10)
project(ooplr7)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Библиотека с NPC
add_library(npc_lib
    src/elf.cpp
    src/dragon.cpp
    src/druid.cpp
    src/npc.cpp
)

# Библиотека с фабрикой/обсерверами
add_library(patterns_lib
    src/factory.cpp
    src/observers.cpp
)
target_link_libraries(patterns_lib PUBLIC npc_lib)

# Основной exe (main.cpp лежит в корне)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE patterns_lib npc_lib)

# Тесты
enable_testing()
add_executable(ooplr7_tests
    test/test_logic.cpp
)
target_link_libraries(ooplr7_tests
    PRIVATE
      patterns_lib
      npc_lib
      GTest::gtest
      GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ooplr7_tests)