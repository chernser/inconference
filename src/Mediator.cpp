//
// Created by Sergey Chernov on 2018-12-05.
//

#include "Mediator.h"
#include "Endpoint.h"

using namespace Endpoints;

namespace Mediation {


    Mediator::Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool) : bufferPool(bufferPool),
                                                                                   endpoints(), listeners() {}
    void Mediator::addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        endpoints.insert(
                std::make_pair(endpoint->getName(), new EndpointHolder({
                                                                               endpoint, bufferPool->borrow()
                                                                       })));
    }

    void Mediator::removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint) {
        auto item = endpoints.find(endpoint->getName());
        if (item != endpoints.end()) {
            auto inputBuffer = ((shared_ptr<struct EndpointHolder>)(*item).second)->inputBuffer;
            bufferPool->release(inputBuffer);
            endpoints.erase(item);
        }

    }

    void Mediator::addListener(const Mediation::MediatorListener *listener) {
        listeners.insert(listeners.begin(), listener);
    }

    void Mediator::start() {

    }

    void Mediator::stop() {

    }

    Mixer::Mixer() : listeners() {}

    void Mixer::addListener(MixerListener *listener) {
        listeners.insert(listeners.begin(), listener);
    }


}

