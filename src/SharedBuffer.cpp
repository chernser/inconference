//
// Created by Sergey Chernov on 2018-12-05.
//

#include "SharedBuffer.h"

namespace Memory {


    unique_ptr<LocalBuffer> LocalBufferPool::allocate(u_int64_t size) {

        return unique_ptr<LocalBuffer>();
    }

    class std::shared_ptr<class Buffer> BufferFactory::createBuffer(u_int64_t size) {
        return shared_ptr<Buffer>();
    }
}