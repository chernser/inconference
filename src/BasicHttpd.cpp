/*
 * BasicHttpd.cpp
 *
 *  Created on: Jan 22, 2019
 *      Author: chernser
 */

#include "BasicHttpd.h"
#include "GenericServer.h"

#include <memory>
#include <functional>

using namespace placeholders;
using namespace std;

namespace InternalHttp
{

    BasicHttpd::BasicHttpd(uint16_t port) 
    {
        this->fdSelector = std::shared_ptr<LibEventFDSelector>(new LibEventFDSelector());
        this->genServer = std::unique_ptr<GenericServer>(
                new GenericServer("0.0.0.0", port, this->fdSelector));

        auto func = std::bind(&BasicHttpd::mainLoop, this);        
        this->mainLoopThread = std::unique_ptr<thread>(new thread(func));        
    }

    void BasicHttpd::mainLoop()
    {
        while (isRunning)
        {
            fdSelector->wakeUp();
            FileDescriptor fd = -1;
            while (genServer->acceptConnection(&fd) != GENSERV_EMPTY_RESULT)
            {
                if (fd != -1)
                {
                    // client accepted
                }
                fd = -1;
            }

            std::this_thread::yield();
        }
    }

    void BasicHttpd::join()
    {
        mainLoopThread->join();
    }

    BasicHttpd::~BasicHttpd()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace Endpoints */
