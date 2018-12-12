cmake_minimum_required(VERSION 3.12)
project(inconference)
enable_testing()


find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14)

#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#add_library(inconference SHARED library.cpp library.h Mediator.cpp Mediator.h )

# Test

#if (APPLE)
#    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
#    add_definitions(-D__GLIBCXX__)
#endif (APPLE)

add_subdirectory(src)

add_subdirectory(${GTEST_LIBRARY}
        ${GTEST_LIBRARY}/googletest/build
        EXCLUDE_FROM_ALL)
add_subdirectory(test)

#add_subdirectory(${GTEST_LIBRARY}
#        ${GTEST_LIBRARY}/googletest/build
#        EXCLUDE_FROM_ALL)

#add_executable(inconferenceTests  test/MediatorTests.cpp)
#
#target_link_libraries(inconferenceTests inconference gtest_main )
