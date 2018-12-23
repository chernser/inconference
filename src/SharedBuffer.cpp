//
// Created by Sergey Chernov on 2018-12-05.
//

#include <unistd.h>

#include <iostream>

#include "SharedBuffer.h"

namespace Memory {


    FixedSizeBufferPool::FixedSizeBufferPool(uint32_t size, uint32_t initialCount, uint32_t maxTotalSize) :
            size(size), initialCount(initialCount), maxTotalSize(maxTotalSize), buffers(), borrowedCount(0) {
        init();
    }

    void FixedSizeBufferPool::init() {
        auto pageSize = static_cast<uint32_t >(sysconf(_SC_PAGE_SIZE));
        uint32_t sizeReminder = size % pageSize;
        size = sizeReminder != 0 ? size - sizeReminder + pageSize : size;
        if (initialCount * size > maxTotalSize) throw std::invalid_argument("size * initialCount > maxTotalSize");

        for (uint32_t  i = 0; i < initialCount; i++) {
            buffers.push_back(shared_ptr<Buffer>(new LocalBuffer(size)));
        }
    }

    shared_ptr<Buffer> FixedSizeBufferPool::borrow() {
        if (buffers.empty()) {
            if (!canAddMore()) {
                throw std::underflow_error("buffer pool underflow");
            }

            buffers.push_back(shared_ptr<Buffer>(new LocalBuffer(size)));
        }

        auto freeBuffer = buffers.front();
        buffers.pop_front();
        borrowedCount++;

        return freeBuffer;
    }

    void FixedSizeBufferPool::release(shared_ptr<Buffer> buffer) {
        if (borrowedCount > 0) {
            borrowedCount--;
            buffers.push_back(buffer);
        }
    }

    bool FixedSizeBufferPool::canAddMore()
    {
        return (borrowedCount + buffers.size() + 1) * size <= maxTotalSize;
    }
}