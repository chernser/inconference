//
// Created by Sergey Chernov on 2018-12-05.
//

#ifndef INCONFERENCE_MEDIATOR_H
#define INCONFERENCE_MEDIATOR_H


#include <zconf.h>
#include <set>
#include <unordered_map>

#include "SharedBuffer.h"
#include "Endpoint.h"


namespace Mediation {




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
    class Mediator {

        const u_int64_t DEFAULT_BUFFER_SIZE = 1024 * 1024;

    public:
        Mediator(const shared_ptr<Memory::BufferFactory> &bufferFactory);

        void addEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        void removeEndpoint(shared_ptr<Endpoints::Endpoint> endpoint);

        const set<shared_ptr<Endpoints::Endpoint>> &getEndpoints() const;

        void start();

        void stop();

        /*
         * Polls information from ready endpoints
         */
        void poll();

        /**
         * Pushes data to endpoints
         */
        void push();


    private:
        shared_ptr<Memory::BufferFactory> bufferFactory;
        std::set<shared_ptr<Endpoints::Endpoint>> endpoints;
        std::unordered_map<std::string, shared_ptr<Memory::Buffer>> endpointInputBuffers;
    };

}

#endif //INCONFERENCE_MEDIATOR_H
