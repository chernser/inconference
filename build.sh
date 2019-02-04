#!/bin/sh

GTEST_DIR=/Users/schernov/tools/googletest/googletest/
LIB_EVENT_DIR=/Users/chernser/tools/libevent-2.0.21-stable/
LIB_EVENT_CORE_VERSION=-2.0
CC=gcc 
CXX=g++

cmake . \
    -DGTEST_LIBRARY=${GTEST_DIR} \
    -DGTEST_INCLUDE_DIR=${GTEST_DIR}/include \
    -DGTEST_MAIN_LIBRARY=gtest \
    -DLIB_EVENT_DIR=${LIB_EVENT_DIR} \
    # -DLIB_EVENT_CORE_VERSION=${LIB_EVENT_CORE_VERSION} 


# DYLD_LIBRARY_PATH=/Users/chernser/tools/libevent-2.0.21-stable/.libs/ ./test/inconferenceTests