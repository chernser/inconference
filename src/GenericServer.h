/*
 * GenericServer.h
 *
 *  Created on: Jan 13, 2019
 *      Author: chernser
 */

#ifndef SRC_GENERICSERVER_H_
#define SRC_GENERICSERVER_H_

#include <string>
#include <stack>
#include "SystemIO.h"

using namespace std;
using namespace SysIO;

namespace GenServer
{

#define GENSERV_OK 0
#define GENSERV_EMPTY_RESULT 1
#define GENSERV_ADDR_IN_USE 1001
#define GENSERV_INTERNAL_ERR 1002

    class GenericServer
    {

    public:
        GenericServer(string address, uint16_t port,
                shared_ptr<FDSelector> selector);
        virtual ~GenericServer();

        /**
         * Creates server socket and does binding.
         * After calling this function - server is ready to accept connections
         */
        uint16_t start();

        /**
         * Accepts new connections if any.
         * fd - pointer to memory where to store client's file descriptor
         */
        uint16_t acceptConnection(FileDescriptor *fd);

        uint16_t nextReadableClient(FileDescriptor *fd);

        uint16_t nextWritableClient(FileDescriptor *fd);

        uint16_t stop();

    private:

        void onClientSocketEvent(FileDescriptor fd, FDEventType eventType);

    private:
        string address;
        uint16_t port;

        int serverSocket;
        shared_ptr<FDSelector> selector;
        std::stack<FileDescriptor> readyForRead;
        std::stack<FileDescriptor> readyForWrite;
    };

} /* namespace Endpoints */

#endif /* SRC_GENERICSERVER_H_ */
