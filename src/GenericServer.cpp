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
#include <errno.h>
#include <functional>
#include <unistd.h>
#include <fcntl.h>

#include "GenericServer.h"

using namespace std;
using namespace placeholders;

namespace GenServer
{

    GenericServer::GenericServer(string address, uint16_t port, shared_ptr<FDSelector> selector) 
    {
        this->selector = selector;
        this->address = address;
        this->port = port;
        serverSocket = -1;
    }

    GenericServer::~GenericServer()
    {
        // TODO Auto-generated destructor stub
    }

    uint16_t GenericServer::start()
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

        int sFlags = ::fcntl(s, F_GETFL);
        if (sFlags == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }
        err = fcntl(s, F_SETFL, sFlags | O_NONBLOCK);
        if (err == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        struct sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(struct sockaddr_in));
        sockAddr.sin_len = (__uint8_t ) sizeof(struct sockaddr_in);
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = INADDR_ANY; // inet_addr(address.c_str());

        err = ::bind(s, (struct sockaddr *) &sockAddr, sizeof(sockAddr));
        if (err == -1)
        {
            return GENSERV_ADDR_IN_USE;
        }

        err = listen(s, 500);
        if (err == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        this->serverSocket = s;

        return GENSERV_OK;
    }

    uint16_t GenericServer::acceptConnection(FileDescriptor *fd)
    {
        struct sockaddr_in clientSockAddr;
        memset(&clientSockAddr, 0, sizeof(struct sockaddr_in));
        unsigned int size = 0;
        int clientSocket = accept(serverSocket,
                (struct sockaddr *) &clientSockAddr, &size);

        if (clientSocket == -1)
        {
            if (errno == EWOULDBLOCK)
            {
                // no connections
                return GENSERV_EMPTY_RESULT;
            }
            else
            {
                return GENSERV_INTERNAL_ERR;
            }
        }

        selector->addFileDescriptor(clientSocket,
                std::bind(&GenericServer::onClientSocketEvent, this, _1, _2));
        if (fd != NULL)
        {
            *fd = clientSocket;
        }
        return GENSERV_OK;
    }

    uint16_t GenericServer::nextReadableClient(FileDescriptor* fd)
    {
        
        if (!readyForReadMutex.try_lock())
        {
            return GENSERV_EMPTY_RESULT;
        }

        std::lock_guard<std::recursive_mutex>(this->readyForReadMutex);        
        if (!readyForRead.empty())
        {
            *fd = readyForRead.top();
            readyForRead.pop();
            return GENSERV_OK;
        }

        return GENSERV_EMPTY_RESULT;
    }

    uint16_t GenericServer::nextWritableClient(FileDescriptor* fd)
    {
        if (!readyForWriteMutex.try_lock())
        {
            return GENSERV_EMPTY_RESULT;
        }

        std::lock_guard<std::recursive_mutex>(this->readyForWriteMutex);
        if (!readyForWrite.empty())
        {
            *fd = readyForWrite.top();
            readyForWrite.pop();
            return GENSERV_OK;
        }

        return GENSERV_EMPTY_RESULT;
    }

    uint16_t GenericServer::stop()
    {
        int err = ::close(this->serverSocket);
        if (err == -1)
        {
            return GENSERV_INTERNAL_ERR;
        }

        return GENSERV_OK;
    }

    void GenericServer::onClientSocketEvent(FileDescriptor fd,
            FDEventType eventType)
    {
        switch (eventType)
        {
        case FD_SEL_BECAME_READABLE:
        {
            std::lock_guard<std::recursive_mutex>(this->readyForReadMutex);
            readyForRead.push(fd);
            break;
        }
        case FD_SEL_BECAME_WRITABLE:
        {
            std::lock_guard<std::recursive_mutex>(this->readyForWriteMutex);
            readyForWrite.push(fd);
            break;
        }
        }
    }

} /* namespace Endpoints */
