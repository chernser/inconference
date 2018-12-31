//
// Created by Sergey Chernov on 2018-12-05.
//

#include "Mediator.h"
#include "Endpoint.h"

using namespace Endpoints;

/**
 * Mediator reads data from endpoints and pushes it to all other endpoints
 * Endpoints are free to implement any logic further.
 *
 * Mediator does only one thing - collect data and notifies others about its readiness
 *
 */
namespace Mediation {


    Mediator::Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool) : bufferPool(bufferPool),
                                                                                   endpoints() {
        this->endpointsIter = endpoints.cend();
    }

    void Mediator::addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        endpoints.insert(
                std::make_pair(endpoint->getName(), new EndpointHolder({
                                                                               endpoint, bufferPool->borrow()
                                                                       })));
    }

    void Mediator::removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        auto item = endpoints.find(endpoint->getName());
        if (item != endpoints.end()) {
            auto inputBuffer = ((shared_ptr<struct EndpointHolder>) (*item).second)->inputBuffer;
            bufferPool->release(inputBuffer);
            endpoints.erase(item);
        }

    }


    void Mediator::doIteration() {

        if (endpoints.size() == 0) {
            return;
        }

        if (endpoints.cend() == endpointsIter) {
            endpointsIter = endpoints.cbegin();
        }

        auto endpoint = endpointsIter->second->endpoint;
        if (!endpoint->getEndpointState().isConnected()) {
            // TODO: remove endpoint - mediator do not care about not connected endpoints
        } else if (!endpoint->isLocal() && endpoint->getEndpointState().hasNewData()) {
            // TODO: notify others
        } else if (endpoint->isLocal() && endpoint->getEndpointState().hasNewData()) {
            // TODO: read data to input buffer
            // if got enough data - notify others
        } else if (endpoint->isLocal() && endpoint->getEndpointState().hasNewData()) {

        }
    }

}

