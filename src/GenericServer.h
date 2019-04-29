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
#include <mutex>

#include "SystemIO.h"

using namespace std;
using namespace SysIO;

namespace GenServer
{

#define GENSERV_OK 0
#define GENSERV_EMPTY_RESULT 1
#define GENSERV_ADDR_IN_USE 1001
#define GENSERV_INTERNAL_ERR 1002

    /**
     * Represents generic server methods.
     * Current implementation is not completely thread-safe.
     *
     * Method #acceptConnection doesn't acquire any lock to guard fd selector.
     * this is done to minimize number of locks.
     *
     * How server loop should be organized to avoid locking:
     *
     * Thread 1: main
     *  1 acceptConnection()
     *  2 selector.wakeUp()
     *
     * Thread 2: reader
     *  1 nextReadableClient(&fd)
     *  2 process(&fd)
     *
     *
     *
     * One thread does control job - it accepts connections and wakes up selector.
     * If acceptor thread is different - there should be lock,
     * so selector is not called from different threads
     *
     * Another threads can safely get next client descriptors for processing.
     *
     */
    class GenericServer
    {

    public:
        GenericServer() {};
        GenericServer(string address, uint16_t port, shared_ptr<FDSelector> selector);
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

        /**
         * Returns next readable client socket. Each call to this method 
         * alters the state of the server.
         * 
         * @param fd - pointer to memory into which client descriptor will be written
         * @return GENSERV_EMPTY_RESULT when no readable file descriptors.
         * @synchronized
         */
        uint16_t nextReadableClient(FileDescriptor *fd);

        /**
         * Returns next writable client socket.
         *
         * @param fd - pointer to memory into which client descriptor will be written
         * @synchronized
         */
        uint16_t nextWritableClient(FileDescriptor *fd);

        uint16_t stop();

    private:

        void onClientSocketEvent(FileDescriptor fd, FDEventType eventType);

    private:
        string address;
        uint16_t port;

        int serverSocket;
        shared_ptr<FDSelector> selector;
        std::recursive_mutex readyForReadMutex;
        std::recursive_mutex readyForWriteMutex;
        std::stack<FileDescriptor> readyForRead;
        std::stack<FileDescriptor> readyForWrite;
    };

} /* namespace Endpoints */

#endif /* SRC_GENERICSERVER_H_ */
