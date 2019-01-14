/*
 * GenericServer.cpp
 *
 *  Created on: Jan 13, 2019
 *      Author: chernser
 */

#include <sys/socket.h>
#include <sys/sockio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "GenericServer.h"

namespace GenServer
{

    GenericServer::GenericServer(string address, uint16_t port, shared_ptr<FDSelector> selector) :
            address(address), port(port), serverSocket(-1), selector(selector)
    {

    }

    GenericServer::~GenericServer()
    {
        // TODO Auto-generated destructor stub
    }

    uint16_t GenericServer::run()
    {

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        // set options
        int value = 1;
        int err = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &value,
                sizeof(value));
        if (err == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        struct sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(struct sockaddr_in));
        sockAddr.sin_len = (__uint8_t ) sizeof(struct sockaddr_in);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = inet_addr(address.c_str());

        err = bind(s, (struct sockaddr *) &sockAddr, sizeof(sockAddr));
        if (err == -1)
        {
            return GENSERV_ADDR_IN_USE;
        }

        err = listen(s, 1000);
        if (err == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        this->serverSocket = s;

//        selector->addFileDescriptor(this->serverSocket, )

        return GENSERV_RUN_OK;
    }

} /* namespace Endpoints */
