cmake_minimum_required(VERSION 3.12)
project(inconference)
enable_testing()

set(CMAKE_CXX_STANDARD 14)


include_directories(BEFORE SYSTEM ${LIB_EVENT_DIR}/include)
link_directories(${LIB_EVENT_DIR}/.libs/)


add_subdirectory(src)
add_subdirectory(test)

