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
namespace Mediation
{

    Mediator::Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool) :
            bufferPool(bufferPool), endpoints()
    {
        this->endpointsIter = endpoints.end();
    }

    void Mediator::addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint)
    {
        auto holder = new EndpointHolder;
        holder->endpoint = endpoint;
        holder->inputBufferA = bufferPool->borrow();
        holder->inputBufferB = bufferPool->borrow();

        endpoints.insert({endpoint->getName(), std::shared_ptr<EndpointHolder>(holder)});
    }

    void Mediator::removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint)
    {
        auto item = endpoints.find(endpoint->getName());
        if (item != endpoints.end())
        {
            auto endpointHolder = ((*item).second).get();
            bufferPool->release(endpointHolder->inputBufferA);
            bufferPool->release(endpointHolder->inputBufferB);
            endpoints.erase(item);
        }

    }

    void Mediator::doIteration()
    {
        if (endpoints.size() == 0)
        {
            return;
        }

        if (endpoints.end() == endpointsIter)
        {
            endpointsIter = endpoints.begin();
        }

        auto endpoint = endpointsIter->second->endpoint;
        if (!endpoint->getEndpointState().isConnected())
        {
            endpointsIter = endpoints.erase(endpointsIter);
        }
        else if (endpoint->getEndpointState().isReadFinished())
        {
            /**
             * There is no more data from prev. frame - buffers swaped.
             */
            for (auto notifyIter = endpoints.cbegin();
                    notifyIter != endpoints.cend(); ++notifyIter)
            {
                if (notifyIter != endpointsIter)
                {
                    notifyIter->second->endpoint->otherSideReady(
                            endpointsIter->second->endpoint,
                            endpointsIter->second->inputBufferB,
                            endpointsIter->second->readyBytes);
                }
            }
        }
        else if (endpoint->isLocal()
                && endpoint->getEndpointState().hasNewData())
        {
            auto inputBuffer = endpointsIter->second->inputBufferA;
            auto readBytes = endpoint->readToBuffer(inputBuffer);
            endpointsIter->second->readyBytes = readBytes;
            if (endpoint->getEndpointState().isReadFinished())
            {
                Mediator::flipInputBuffers(endpointsIter->second.get());
            }
        }

        ++endpointsIter;
    }

}

