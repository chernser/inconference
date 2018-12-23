//
// Created by Sergey Chernov on 2018-12-05.
//

#include "Mediator.h"
#include "Endpoint.h"

using namespace Endpoints;

namespace Mediation {


    Mediator::Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool) : bufferPool(bufferPool),
        endpoints(), endpointInputBuffers() {}

    void Mediator::addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        endpoints.insert(endpoint);
        endpointInputBuffers.insert(
                std::make_pair(endpoint->getName(), bufferPool->borrow()));
    }

    void Mediator::removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        endpoints.erase(endpoint);
    }

    void Mediator::start() {

    }

    void Mediator::stop() {

    }

    const set<shared_ptr<Endpoints::Endpoint>> &Mediator::getEndpoints() const {
        return endpoints;
    }

    void Mediator::poll() {
        for (const auto &endpoint : endpoints) {
            if (endpoint->isLocal() && endpoint->getEndpointState().hasNewData()) {
                auto inputBuffer = endpointInputBuffers.find(endpoint->getName());
                // TODO: read from endpoint 
            }
        }
    }

    void Mediator::push() {

    }

}

