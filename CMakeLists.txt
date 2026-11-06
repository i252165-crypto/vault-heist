cmake_minimum_required(VERSION 3.16)
project(VaultHeist LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # for clangd / editor

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

option(VH_SANITIZE   "Build with AddressSanitizer + UBSan" ON)
option(VH_BUILD_GAME "Build the SFML game executable"      OFF)

# ---------- warnings and sanitizers ----------
if(NOT MSVC)
  add_compile_options(-Wall -Wextra -Wpedantic)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2)
  else()
    add_compile_options(-g -O0)
  endif()
  if(VH_SANITIZE AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
  endif()
endif()

# ---------- model library (NO SFML dependency) ----------
file(GLOB_RECURSE MODEL_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/events/*.cpp
    ${CMAKE_SOURCE_DIR}/src/model/*.cpp
)

add_library(model STATIC ${MODEL_SOURCES})
target_include_directories(model PUBLIC ${CMAKE_SOURCE_DIR}/src)

# ---------- unit tests (link model only) ----------
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tests/*.cpp)

if(TEST_SOURCES)DynamicArray
  add_executable(tests ${TEST_SOURCES})
  target_link_libraries(tests PRIVATE model)
  target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/tests)

  enable_testing()
  add_test(NAME all_unit_tests COMMAND tests)
endif()

# ---------- benchmarks (enable when bench/ exists; GUI-free, Release build) ----------
# file(GLOB BENCH_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/bench/*.cpp)
# add_executable(bench ${BENCH_SOURCES})
# target_link_libraries(bench PRIVATE model)

# ---------- game (SFML) ----------
if(VH_BUILD_GAME)
  # SFML 3.x:
  find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)
  # SFML 2.6.x instead:
  # find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

  file(GLOB_RECURSE GUI_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/gui/*.cpp)
  add_executable(game ${GUI_SOURCES})
  target_link_libraries(game PRIVATE model SFML::Graphics SFML::Window SFML::System)
  # SFML 2.6: link sfml-graphics sfml-window sfml-system instead
endif()
