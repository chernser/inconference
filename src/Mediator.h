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


    class MixerListener {

    public:
        virtual void onOutFrameReady(std::vector<shared_ptr<Endpoints::Endpoint>> forEndpoints) = 0;

    };

    class Mixer {

    public:

        Mixer();

        void addListener(MixerListener *listener);

    private:
        std::vector<MixerListener *> listeners;
    };


    class MediatorListener {

    public:
        virtual void onEdnpointAdded(shared_ptr<Endpoints::Endpoint> endpoint) = 0;

        virtual void onEndpointRemoved(shared_ptr<Endpoints::Endpoint> endpoint) = 0;

        virtual void onDataFrameReady(shared_ptr<Endpoints::Endpoint> endpoint,
                                      shared_ptr<Memory::Buffer> inputBuffer) = 0;
    };
    /**
     * Each mediator works with single distribution task.
     * If it is conference application, than one mediator handles one conference.
     * If it is one chat application, than one mediator handles one dialog
     *
     * If two endpoints of same mediation process are connected to different servers, than
     * mediator for their dialog has two instances on two different processing units (host, virtual machine)
     *
     * Mediation algorithm:
     *
     * Poll phase
     *   1. Take each local endpoint which has data to read (it means complete chunk of data is ready)
     *   2. Copy chunk from endpoint buffer to shared one (distributed buffer) allocated for each endpoint
     *   3. Notify mixer
     *
     * Push phase
     *   1. Check mixer state
     *   2. Notify endpoint that mixer output is ready
     *
     * Mixer is running on each node.
     * Only local endpoints are notified about readiness of mixer result
     *
     *
     */
#define DEFAULT_BUFFER_SIZE (1024 * 1024 * 2)

    class Mediator {

    public:

        Mediator(const shared_ptr<Memory::FixedSizeBufferPool> bufferPool);

        void addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        void removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        void addListener(const MediatorListener *listener);

        void start();

        void stop();

        void doReadData();

        void doWriteData();

    private:

        struct EndpointHolder {
            shared_ptr<Endpoints::Endpoint> endpoint;
            shared_ptr<Memory::Buffer> inputBuffer;
        };
        shared_ptr<Memory::FixedSizeBufferPool> bufferPool;

        std::unordered_map<std::string, shared_ptr<struct EndpointHolder>> endpoints;

        std::vector<MediatorListener *> listeners;
    };

}

#endif //INCONFERENCE_MEDIATOR_H
