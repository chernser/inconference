/*
 * GenericServer.h
 *
 *  Created on: Jan 13, 2019
 *      Author: chernser
 */

#ifndef SRC_GENERICSERVER_H_
#define SRC_GENERICSERVER_H_

#include <string>
#include "SystemIO.h"

using namespace std;
using namespace SysIO;

namespace GenServer
{

#define GENSERV_RUN_OK 0
#define GENSERV_ADDR_IN_USE 1001
#define GENSERV_INTERNAL_ERR 1002

    class GenericServer
    {
    public:
        GenericServer(string address, uint16_t port,
                shared_ptr<FDSelector> selector);
        virtual ~GenericServer();

        /**
         * Runs server in current thread,
         * In case of failure returns error
         */
        uint16_t run();
    private:
        string address;
        uint16_t port;
        int serverSocket;
        shared_ptr<FDSelector> selector;
    };

} /* namespace Endpoints */

#endif /* SRC_GENERICSERVER_H_ */
