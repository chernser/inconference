#!/bin/sh

GTEST_DIR=/Users/schernov/tools/googletest/googletest/

cmake . \
    -DGTEST_LIBRARY=${GTEST_DIR} \
    -DGTEST_INCLUDE_DIR=${GTEST_DIR}/include \
    -DGTEST_MAIN_LIBRARY=gtest
