//
// Created by Sergey Chernov on 2018-12-30.
//

#ifndef INCONFERENCE_MEDIATORTESTHELPER_H
#define INCONFERENCE_MEDIATORTESTHELPER_H


#include "../src/Endpoint.h"

using namespace Endpoints;

class SimpleTestEndpoint : public Endpoints::Endpoint {

public:

    SimpleTestEndpoint(const string &name) : Endpoint(name, true), readBytes(0), readyBytes(0) {
    }

    void connect() {
        this->setState(ENDPOINT_STATE_CONNECTED);
    }

    void otherSideReady(shared_ptr<Endpoint> otherEndpoint, shared_ptr<Buffer> buffer, uint32_t readyBytes) override {
        this->inBuffer = shared_ptr<uint8_t>(new uint8_t[buffer->capacity()]{});
        buffer->copy(this->inBuffer.get());
        this->readBytes = readyBytes;
    }

    uint32_t readToBuffer(shared_ptr<Buffer> buffer) override {
        buffer->copyFrom(localBuffer.get(), 0, readyBytes);
        setState(ENDPOINT_STATE_CONNECTED | ENDPOINT_STATE_DATA_CHUNK_COMPLETE);
        return readyBytes;
    }

    void setReadyData(shared_ptr<uint8_t> data, uint32_t size) {
        this->localBuffer = data;
        this->readyBytes = size;
        setState(ENDPOINT_STATE_CONNECTED | ENDPOINT_STATE_HAS_DATA_TO_READ);
    }

    void setInBuffer(shared_ptr<uint8_t> inBuffer) {
        this->inBuffer = inBuffer;
    }

    uint32_t getReadBytes() {
        return readBytes;
    }

    shared_ptr<uint8_t> getInBuffer() {
        return inBuffer;
    }

private:

    shared_ptr<uint8_t> localBuffer;
    uint32_t readyBytes;

    shared_ptr<uint8_t> inBuffer;
    uint32_t readBytes;

};

#endif //INCONFERENCE_MEDIATORTESTHELPER_H
