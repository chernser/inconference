//
// Created by Sergey Chernov on 2018-12-05.
//

#include <unistd.h>

#include <iostream>

#include "SharedBuffer.h"

namespace Memory {


    FixedSizeBufferPool::FixedSizeBufferPool(uint32_t size, uint32_t initialCount, uint32_t maxTotalSize) :
            size(size), initialCount(initialCount), maxTotalSize(maxTotalSize), buffers() {
        init();
    }

    void FixedSizeBufferPool::init() {
        auto pageSize = static_cast<uint32_t >(sysconf(_SC_PAGE_SIZE));
        size = size - (size % pageSize) + pageSize;
        if (initialCount * size > maxTotalSize) throw std::invalid_argument("size * initialCount > maxTotalSize");

        for (uint32_t  i = 0; i < initialCount; i++) {
            buffers.push_back(shared_ptr<Buffer>(new LocalBuffer(size)));
        }
    }

    shared_ptr<Buffer> FixedSizeBufferPool::borrow() {
        if (buffers.empty()) {
            throw std::exception();
        }


        auto freeBuffer = buffers.front();
        buffers.pop_front();

        return freeBuffer;
    }

    void FixedSizeBufferPool::release(shared_ptr<Buffer> buffer) {

        if ((buffers.size() + 1) * size > maxTotalSize) {
            // do nothing
        } else {
            buffers.push_back(buffer);
        }
    }
}