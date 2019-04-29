/*
 * GenericServerTests.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: chernser
 */

#include <memory>
#include <stdio.h>
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <thread>

#include <unistd.h>

#include "gtest/gtest.h"

#include "../src/GenericServer.h"

using namespace GenServer;
using namespace SysIO;

void assertReadableFdIs(int expected, GenericServer *server ) ;

int connect_client()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return -1;
    }

    int sFlags = ::fcntl(sock, F_GETFL);
    if (sFlags == -1)
    {
        return -1;
    }
    int err = fcntl(sock, F_SETFL, sFlags | O_NONBLOCK);
    if (err == -1)
    {
        return -1;
    }

    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    // if def unix 
        // sockAddr.sin_len = (__uint8_t ) sizeof(struct sockaddr_in);
    // endif unix 
    
    inet_pton(AF_INET, "127.0.0.1", &sockAddr.sin_addr);
    sockAddr.sin_port = htons(9090);
    sockAddr.sin_family = AF_INET;

    err = ::connect(sock, (sockaddr *) &sockAddr, sizeof(sockaddr_in));
    if (err == -1)
    {
        if (errno == EINPROGRESS)
        {
            return sock;
        }
        return -1;
    } 
    return sock;
}

TEST(single_threaded_server, full_cycle)
{
    auto fdSelector = std::shared_ptr<FDSelector>(new LibEventFDSelector());
    auto server = std::unique_ptr<GenericServer>(new GenericServer("127.0.0.1", 9090, fdSelector));

    ASSERT_EQ(GENSERV_OK, server->start());

    int clientFd1 = connect_client();
    ASSERT_NE(-1, clientFd1);

    int acceptedFd = -1;
    int attempts = 10;
    int acceptResult = server->acceptConnection(&acceptedFd);
    while (acceptResult == GENSERV_EMPTY_RESULT && attempts > 0)
    {
        acceptResult = server->acceptConnection(&acceptedFd);
        ASSERT_NE(GENSERV_INTERNAL_ERR, acceptResult);
        --attempts;
        usleep(1000 * 1000 * 10);
    }

    ASSERT_NE(-1, acceptedFd);

    const char *msg = "hello";
    write(clientFd1, (void*) msg, 5);

    fdSelector->wakeUp();    
    assertReadableFdIs(acceptedFd, server.get());

    {
        fdSelector->wakeUp();
        int readableFd = -1;    
        ASSERT_EQ(GENSERV_EMPTY_RESULT, server->nextReadableClient(&readableFd));

    }

    {
        int writableFd = -1;
        server->nextWritableClient(&writableFd);
    }    

    ASSERT_EQ(GENSERV_OK, server->stop());
}


inline void assertReadableFdIs(int expected, GenericServer *server ) 
{
    int readableFd = -1;        
    server->nextReadableClient(&readableFd);
    ASSERT_EQ(readableFd, expected);
}
