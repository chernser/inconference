
//
// Created by Sergey Chernov on 2018-12-05.
//
#include <memory>
#include <unistd.h>

#include "../src/SharedBuffer.h"



#include "gtest/gtest.h"
using namespace Memory;

TEST(initPool, validParameters) {

    auto bufferPool = shared_ptr<FixedSizeBufferPool>(new FixedSizeBufferPool(4090, 10, 4090 * 100));
    auto pageSize = static_cast<uint32_t >(sysconf(_SC_PAGE_SIZE));

    ASSERT_EQ(pageSize, bufferPool->getBufferSize());
}

TEST(initPool, invalidParameters) {

    try {
        auto bufferPool = shared_ptr<FixedSizeBufferPool>(new FixedSizeBufferPool(4090, 10, 10));
        GTEST_NONFATAL_FAILURE_("No exception");
    } catch (exception &e) {
        cout << e.what() << "\n";
    }
}