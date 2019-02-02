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

#include "gtest/gtest.h"

#include "../src/GenericServer.h"

using namespace GenServer;
using namespace SysIO;

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
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_len = (__uint8_t ) sizeof(struct sockaddr_in);
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(65002);

    err = ::connect(sock, (sockaddr *) &sockAddr, sockAddr.sin_len);
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

TEST(single_threaded_server, full_cicle)
{
    auto fdSelector = LibEventFDSelector();

    auto server = std::unique_ptr<GenericServer>(new GenericServer("127.0.0.1", 65002,
            std::shared_ptr<FDSelector>(&fdSelector)));

    ASSERT_EQ(GENSERV_OK, server->start());

    printf("Server started\n");
    int clientFd1 = connect_client();
    printf("Client fd: %d\n", clientFd1);
    ASSERT_NE(-1, clientFd1);

    printf("Connecting client on server side\n");
    int acceptedFd = -1;
    while (server->acceptConnection(&acceptedFd) == GENSERV_EMPTY_RESULT)
    {

    }

    //    ASSERT_EQ(GENSERV_OK, server.acceptConnection(&acceptedFd));
    printf("Accepted fd: %d\n", acceptedFd);
    ASSERT_NE(-1, acceptedFd);

    const char *msg = "hello";
    write(clientFd1, (void*) msg, 5);

    fdSelector.wakeUp();
    int readableFd = -1;    
    server->nextReadableClient(&readableFd);
    printf("Readable fd: %d\n", readableFd);
    ASSERT_EQ(readableFd, acceptedFd);

    ASSERT_EQ(GENSERV_OK, server->stop());

    printf("Test done\n");
}

