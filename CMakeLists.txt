cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(deindent)

set(DEINDENT_VERSION 1.2.0)
set(BUILD_SHARED_LIBS OFF)

if (GIT_HASH)
  set(DEINDENT_VERSION "${DEINDENT_VERSION}+${GIT_HASH}")
endif()

configure_file(
  deindent_version.in.h
  "${PROJECT_BINARY_DIR}/deindent_version.h"
)

configure_file(
  deindent_version.in.txt
  "${PROJECT_BINARY_DIR}/deindent_version.txt"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(deps/cmake/sanitizers/address.cmake)
endif()

include(deps/cmake/c99-gnu.cmake)
include_directories(deps)
include_directories("${PROJECT_BINARY_DIR}")

if (PROJECT_IS_TOP_LEVEL AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()
  add_compile_options(-D UNIT_TESTING)
  include(deps/cmake/cmocka-git.cmake)

  add_cmocka_test(deindent_test
    SOURCES deindent_test.c
    LINK_LIBRARIES cmocka-static deindent stb
  )

  add_cmocka_test(util_test
    SOURCES util_test.c
    LINK_LIBRARIES cmocka-static util
  )
endif()

add_library(xmalloc EXCLUDE_FROM_ALL deps/core/xmalloc.c)
add_library(stb EXCLUDE_FROM_ALL deps/stb/stb.c)

add_library(args EXCLUDE_FROM_ALL args.c deps/commander/src/commander.c)
target_link_libraries(args xmalloc util)

add_library(util EXCLUDE_FROM_ALL util.c)

add_library(sds EXCLUDE_FROM_ALL deps/sds/sds.c)
target_compile_options(sds PRIVATE -Wno-shadow)

add_library(deindent EXCLUDE_FROM_ALL deindent.c)
target_link_libraries(deindent sds)

add_executable(main main.c)
target_link_libraries(main args sds deindent util)
