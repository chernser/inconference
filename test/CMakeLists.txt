

find_package(GTest REQUIRED)

include_directories(${GTEST_ROOT}/include ${GTEST_ROOT})
link_directories(${GTEST_ROOT}/../lib)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

add_executable(inconferenceTests  
	MediatorTests.cpp 
	SharedBufferTests.cpp
	FDSelectorTests.cpp
	MediatorTestHelper.h
	GenericServerTests.cpp)

target_link_libraries(inconferenceTests inconference gtest gtest_main event_core${LIB_EVENT_CORE_VERSION} )
