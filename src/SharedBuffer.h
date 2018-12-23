//
// Created by Sergey Chernov on 2018-12-05.
//

#ifndef INCONFERENCE_SHAREDBUFFER_H
#define INCONFERENCE_SHAREDBUFFER_H

#include <memory>
#include <cctype>
#include <stack>
#include <unistd.h>
#include <cstdint>


using namespace std;

namespace Memory {


    class Buffer {
    public:
        explicit Buffer() {}

        virtual ~Buffer() = default;
    protected:
    };

    /**
     * Represents local buffer
     */
    class LocalBuffer : public Buffer {


    public:
        LocalBuffer(uint32_t size) {
            buffer = new uint8_t[size];
        }


        uint8_t read() {
            readPosition++;
            return readPosition < size ? buffer[readPosition] : throw out_of_range("Buffer ended");
        }


        void copy(uint8_t *dest) {
            memcpy(dest, buffer, size);
        }

        void write(uint8_t byte) {
            writePosition++;
            writePosition < size ? buffer[writePosition] = byte : throw out_of_range("Buffer ended");
        }

        uint32_t copyFrom(uint8_t *src) {
            memcpy(buffer, src, size);
        }

        uint32_t capacity() {
            return size;
        }

    private:
        volatile uint32_t readPosition = 0;
        volatile uint32_t writePosition = 0;
        uint32_t size;
        uint8_t *buffer;
    };

    class FixedSizeBufferPool {

    public:

        /**
         * Constructs the fixed size buffer pool which returns only fixed size buffer.
         * When new buffer size is required - new pool is created.
         *
         * @param size - size of buffer
         * @param initialCount - initial number of buffers
         * @param maxTotalSize - max total size of buffers allocated by pool
         */
        explicit FixedSizeBufferPool(uint32_t size, uint32_t initialCount, uint32_t maxTotalSize);

        ~FixedSizeBufferPool() {}

        shared_ptr<Buffer> borrow();

        void release(shared_ptr<Buffer> buffer);

        uint32_t getBufferSize() { return size; };

        uint32_t getBorrowedCount() {};

        uint32_t getAllocatedCount() {};

    private:

        void init();

        uint32_t size;
        uint32_t initialCount;
        uint32_t maxTotalSize;

        std::deque<shared_ptr<Buffer>> buffers;
    };


}
#endif //INCONFERENCE_SHAREDBUFFER_H
