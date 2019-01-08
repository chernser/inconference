/*
 * SystemIO.h
 *
 *  Created on: Jan 6, 2019
 *      Author: chernser
 */

#ifndef SRC_SYSTEMIO_H_
#define SRC_SYSTEMIO_H_

#include <event2/event.h>
#include <unordered_map>
#include <memory>
#include <functional>

using namespace std;

namespace SysIO
{

    typedef int FileDescriptor;

    enum FDEventType
    {
        FD_SEL_BECAME_READABLE = 1, FD_SEL_BECAME_WRITABLE
    };

    typedef void (*onFDStateChangeCallback(FileDescriptor fd,
            FDEventType eventType));

class    FDSelector
    {

    public:

        FDSelector() :
                fdEventMap()
        {
        }

        virtual ~FDSelector()
        {

        }

        void notifyFDStateChanged(FileDescriptor fd, FDEventType eventType);

        virtual void addFileDescriptor(FileDescriptor fd, std::function<void(FileDescriptor fd, FDEventType)> cb) = 0;
        virtual void removeFileDescriptor(FileDescriptor fd) = 0;
        virtual void wakeUp() = 0;

    protected:
        struct FdWrapper
        {
            event *event;
            std::function<void(FileDescriptor fd, FDEventType)> stateCallback;
        };
        unordered_map<FileDescriptor, shared_ptr<struct FdWrapper>> fdEventMap;
    };

    class LibEventFDSelector: public FDSelector
    {

    public:
        LibEventFDSelector() :
                eventBase(event_base_new())
        {
        }

        virtual ~LibEventFDSelector()
        {
            event_base_free(eventBase);
        }

        void addFileDescriptor(FileDescriptor fd, std::function<void(FileDescriptor fd, FDEventType)> cb) override;
        void removeFileDescriptor(FileDescriptor fd) override;
        void wakeUp() override;

    private:
        event_base *eventBase;
        static void eventCallback(evutil_socket_t fd, short event,
                FDSelector *selector);

    };

    class SystemIO
    {
    public:
        SystemIO();
        virtual ~SystemIO();
    };

} /* namespace Endpoints */

#endif /* SRC_SYSTEMIO_H_ */
