/*
 * SystemIO.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: chernser
 */

#include "SystemIO.h"

#include <event2/event.h>
#include <memory>
#include <unordered_map>
#include <functional>

//using namespace std;

namespace SysIO
{

    SystemIO::SystemIO()
    {
        // TODO Auto-generated constructor stub

    }

    SystemIO::~SystemIO()
    {
        // TODO Auto-generated destructor stub
    }

    void FDSelector::notifyFDStateChanged(FileDescriptor fd,
            FDEventType eventType)
    {
        auto eventIter = fdEventMap.find(fd);
        auto wrapper = eventIter->second;
        if (eventIter != fdEventMap.end() && wrapper->stateCallback != NULL)
        {
            eventIter->second->stateCallback(fd, eventType);
        }
    }

    void LibEventFDSelector::addFileDescriptor(FileDescriptor fd,
            std::function<onFDStateChangeCallback> cb)
    {
        auto event = event_new(this->eventBase, fd, EV_READ | EV_WRITE,
                (event_callback_fn) &LibEventFDSelector::eventCallback, this);

        if (event_add(event, NULL) == 0)
        {
            auto wrapper = shared_ptr<FdWrapper>(new FdWrapper
            { event });
            wrapper->stateCallback = cb;
            auto inserted = fdEventMap.insert(make_pair(fd, wrapper));
            if (!inserted.second)
            {
                event_free(event);
                throw exception();
            }
        }
        else
        {
            event_free(event);
        }
    }

    void LibEventFDSelector::removeFileDescriptor(FileDescriptor fd)
    {
        auto eventIter = fdEventMap.find(fd);
        if (eventIter != fdEventMap.end())
        {
            event_free(eventIter->second->event);
            fdEventMap.erase(eventIter);
        }
    }

    void LibEventFDSelector::wakeUp()
    {
        event_base_loop(eventBase, 0);
    }

    void LibEventFDSelector::eventCallback(int fd, short event,
            FDSelector* selector)
    {
        short evEvents[] = { EV_READ, EV_WRITE, EV_TIMEOUT};
        FDEventType fdEvents[] =
        { FD_SEL_BECAME_READABLE, FD_SEL_BECAME_WRITABLE, FD_SEL_TIMEOUT };
        int size = sizeof(evEvents) / sizeof(evEvents[0]);

        for (int i = 0; i < size; i++) {
            if ((event & evEvents[i]) != 0) {
                selector->notifyFDStateChanged(fd, fdEvents[i]);
            }
        }
    }

} /* namespace Endpoints */

