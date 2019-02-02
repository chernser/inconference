/*
 * BasicHttpd.h
 *
 *  Created on: Jan 22, 2019
 *      Author: chernser
 */

#ifndef SRC_BASICHTTPD_H_
#define SRC_BASICHTTPD_H_

#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <array>

#include "GenericServer.h"

using namespace std;
using namespace GenServer;

namespace InternalHttp
{

    class HttpRequest {



    public:
        HttpRequest();
        virtual ~HttpRequest();


    };

    class Handler
    {

    public:
        Handler(string method, string path);

        virtual ~Handler();



    };

    /**
     * The purpose of this basic httpd is to provide very few necessary functions to build service.
     * This is not full implementation of HTTP protocol, because current library is not about it.
     * This embeded server may be used for simple configuration etc.
     */
    class BasicHttpd
    {


    public:

        BasicHttpd(uint16_t port);
        virtual ~BasicHttpd();

        void join();

    private:
        void mainLoop();

    protected:
        shared_ptr<LibEventFDSelector> fdSelector;
        unique_ptr<GenericServer> genServer;
        unique_ptr<thread> mainLoopThread;
        atomic_bool isRunning;
        std::array<thread, 5> workers;
    };

} /* namespace Endpoints */

#endif /* SRC_BASICHTTPD_H_ */
