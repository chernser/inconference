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

#include <iostream>


using namespace std;

namespace Memory {


    class Buffer {
    public:
        explicit Buffer() {}
        virtual ~Buffer() = default;

        /**
         * Reads single byte from buffer. Not to be used for copying data from one mem block to another.
         * Read position is incremented by 1;
         * @return byte
         */
        virtual uint8_t read() = 0;

        /**
         * Copies content of the buffer (check @link Buffer#capacity())
         *
         * @param dest
         */
        virtual void copy(uint8_t *dest) = 0;

        /**
         * Writes one byte. Not to be used for copying data from one mem block to another
         * @param byte
         */
        virtual void write(uint8_t byte) = 0;

        /**
         * Copies chunk of data from src start of offset position and ends after copying len bytes
         *
         * @param src - source mem block
         * @param offset - offset from the source mem block
         * @param len - length to copy
         * @return
         */
        virtual void copyFrom(uint8_t *src, uint32_t offset, uint32_t len) = 0;

        virtual uint32_t capacity() = 0;

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

        ~ LocalBuffer(){
            cout << "local buffer is destroyed\n";
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

        void copyFrom(uint8_t *src, uint32_t offset, uint32_t len) {
            memcpy(buffer, src + offset, len);
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

        uint32_t getAvailableCount() {
            return buffers.size() +
                    (maxTotalSize / size - ((uint32_t)buffers.size() + borrowedCount));
        }
    private:

        void init();
        bool canAddMore();

        uint32_t size;
        const uint32_t initialCount;
        const uint32_t maxTotalSize;

        /**
         * Free buffers.
         * buffers are taken from front, returns to back
         */
        std::deque<shared_ptr<Buffer>> buffers;

        volatile uint32_t borrowedCount;

    };


}
#endif //INCONFERENCE_SHAREDBUFFER_H
