
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

TEST(borrow, allCases) {

    auto pageSize = static_cast<uint32_t >(sysconf(_SC_PAGE_SIZE));
    auto bufferPool = shared_ptr<FixedSizeBufferPool>(new FixedSizeBufferPool(pageSize, 0, pageSize * 3));

    for (int i = 0; i < 3; i++) {
        shared_ptr<Buffer> buffer1 = bufferPool->borrow();
    }

    try {
        bufferPool->borrow();
        GTEST_NONFATAL_FAILURE_("No exception");
    } catch (exception &e) {
        cout << e.what() << "\n";
    }
}

TEST(release, allCases) {

    auto pageSize = static_cast<uint32_t >(sysconf(_SC_PAGE_SIZE));
    auto bufferPool = shared_ptr<FixedSizeBufferPool>(new FixedSizeBufferPool(pageSize, 0, pageSize * 3));

    {
        auto *buffers = new shared_ptr<Buffer>[3];
        for (int i = 0; i < 3; i++) {
            buffers[i] = bufferPool->borrow();
        }

        GTEST_ASSERT_EQ(0, bufferPool->getAvailableCount());

        bufferPool->release(buffers[0]);
        GTEST_ASSERT_EQ(1, bufferPool->getAvailableCount());
        bufferPool->release(buffers[1]);
        GTEST_ASSERT_EQ(2, bufferPool->getAvailableCount());
    }

}