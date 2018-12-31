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
#define ENDPOINT_STATE_DATA_CHUNK_COMPLETE 4
#define ENDPOINT_STATE_HAS_DATA_TO_READ 8
#define ENDPOINT_STATE_BUSY_WRITING 16


    class EndpointState {

    public:

        void setState(u_int8_t state) {
            this->state = state;
        }

        /**
         *  Determines if endpoint is connected
         *
         * @return true - if connected
         *         false - if not connected
         */
        bool isConnected() const {
            return (state & ENDPOINT_STATE_CONNECTED) != 0;
        }

        /**
         * Determines if endpoint has new data chunk of to read
         *
         * @return true - if there is some data for reading
         *         false - if there is no new data for reading
         */
        bool hasNewData() const {
            return (state & ENDPOINT_STATE_HAS_DATA_TO_READ) != 0;
        }


        /**
         * Determines if reading of the current chunk is finished
         * and data is ready to be sent to other endpoints
         *
         * @return true - if data is read completely
         *         false - if data reading is not completed
         */
        bool isReadFinished() const {
            return (state & ENDPOINT_STATE_DATA_CHUNK_COMPLETE) != 0;
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

        virtual void
        otherSideReady(shared_ptr<Endpoint> otherEndpoint, shared_ptr<Buffer> buffer, uint32_t readyBytes) {};

        virtual uint32_t readToBuffer(shared_ptr<Buffer> buffer) { return 0; };

        const EndpointState &getEndpointState() const;

        void setState(u_int8_t state) {
            endpointState.setState(state);
        }

        virtual ~Endpoint() = default;

    private:
        string name;

        EndpointState endpointState;

        bool local;
    };


}

#endif //INCONFERENCE_ENDPOINT_H
