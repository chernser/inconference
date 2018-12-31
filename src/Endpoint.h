//
// Created by Sergey Chernov on 2018-12-05.
//

#ifndef INCONFERENCE_ENDPOINT_H
#define INCONFERENCE_ENDPOINT_H

#include <string>

#include "SharedBuffer.h"

using namespace std;
using namespace Memory;

namespace Endpoints {

#define ENDPOINT_STATE_CONNECTING 1
#define ENDPOINT_STATE_CONNECTED 2
#define ENDPOINT_STATE_COMPLETE_READING 4
#define ENDPOINT_STATE_BUSY_READING 6
#define ENDPOINT_STATE_BUSY_WRITING 8



    class EndpointState {

    public:

        bool isConnected() const {
            return (state & ENDPOINT_STATE_CONNECTED) != 0;
        }

        bool hasNewData() const {
            return (state & ENDPOINT_STATE_COMPLETE_READING) != 0;
        }

        bool isReady() const {
            return (state & ENDPOINT_STATE_CONNECTED) != 0 || (state & ENDPOINT_STATE_COMPLETE_READING) != 0;
        }



    private:

        u_int8_t state = 0;
    };

    /**
     * Defines endpoint which can send or receive data
     * Instance holds state of the endpoint
     *
     */
    class Endpoint {


    public:

        explicit Endpoint(string name, bool isLocal);

        const string &getName() const;

        bool isLocal() const;

        virtual void otherSideReady(shared_ptr<Endpoint> otherEndpoint, shared_ptr<Buffer> buffer) ;

        const EndpointState &getEndpointState() const;

        virtual ~Endpoint() = default;

    private:
        string name;

        EndpointState endpointState;

        bool local;
    };


}

#endif //INCONFERENCE_ENDPOINT_H
