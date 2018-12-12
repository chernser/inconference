//
// Created by Sergey Chernov on 2018-12-05.
//

#ifndef INCONFERENCE_SHAREDBUFFER_H
#define INCONFERENCE_SHAREDBUFFER_H

#include <memory>
#include <cctype>

using namespace std;

namespace Memory {


    class BufferFactory {

    public:

        class std::shared_ptr<class Buffer> createBuffer(u_int64_t size);
    };


    class Buffer {


    public:
        virtual u_int8_t *getPointer(u_int32_t startPosition) = 0;

        virtual ~Buffer() = default;

    };


    /**
     * Represents local buffer
     */
    class LocalBuffer : public Buffer {


    public:


    private:
        u_int8_t *buffer;
    };

    /**
     * Represents pool of local buffers
     *
     *
     */
    class LocalBufferPool {

    public:
        unique_ptr<LocalBuffer> allocate(u_int64_t size);

    };

    class SharedBuffer {


    };

}
#endif //INCONFERENCE_SHAREDBUFFER_H
