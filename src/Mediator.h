//
// Created by Sergey Chernov on 2018-12-05.
//

#ifndef INCONFERENCE_MEDIATOR_H
#define INCONFERENCE_MEDIATOR_H


#include <zconf.h>
#include <set>
#include <unordered_map>
#include <vector>

#include "SharedBuffer.h"
#include "Endpoint.h"


namespace Mediation {


#define DEFAULT_BUFFER_SIZE (1024 * 1024 * 2)

    class Mediator {

    public:

        Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool);

        void addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        void removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        void doIteration();

        int getNumberOfEndpoints() {
            return this->endpoints.size();
        }

    private:

        struct EndpointHolder {
            shared_ptr<Endpoints::Endpoint> endpoint;
            shared_ptr<Memory::Buffer> inputBufferA;
            shared_ptr<Memory::Buffer> inputBufferB;

            uint32_t readyBytes;
        };

        static void flipInputBuffers(EndpointHolder *endpointHolder) {
            auto bufferA = endpointHolder->inputBufferA;
            endpointHolder->inputBufferA = endpointHolder->inputBufferB;
            endpointHolder->inputBufferA = bufferA;
        }

        shared_ptr<Memory::FixedSizeBufferPool> bufferPool;

        std::unordered_map<std::string, shared_ptr<struct EndpointHolder>> endpoints;

        std::unordered_map<std::string, shared_ptr<struct EndpointHolder>>::iterator endpointsIter;
    };

}

#endif //INCONFERENCE_MEDIATOR_H
