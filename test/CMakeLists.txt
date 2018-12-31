include_directories(${GTEST_ROOT}/include ${GTEST_ROOT})

find_package(GTest REQUIRED)

link_directories(${GTEST_ROOT}/../lib)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(inconferenceTests  MediatorTests.cpp SharedBufferTests.cpp MediatorTestHelper.h)

target_link_libraries(inconferenceTests inconference gtest gtest_main )
