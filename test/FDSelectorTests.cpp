/*
 * FDSelectorTests.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: chernser
 */

#include <memory>
#include <stdio.h>
#include <functional>

#include "../src/SystemIO.h"

#include "gtest/gtest.h"

using namespace SysIO;
using namespace placeholders;

class SelectorCallback
{
public:
    void onFDStateChangeCallback(FileDescriptor fd, FDEventType eventType)
    {
        printf("fd %d triggered %d", fd, eventType);
    }
};

TEST(selecting_fd, libeven_singleThread_2_fds)
{
    auto selector = std::unique_ptr<LibEventFDSelector>(
            new LibEventFDSelector());
    auto selectorCallback = std::unique_ptr<SelectorCallback>(
            new SelectorCallback());

    auto f1 = fopen("/tmp/test_file_1", "rw+");
    auto f2 = fopen("/tmp/test_file_2", "rw+");
    auto f1no = fileno(f1);
    auto f2no = fileno(f2);

    selector->addFileDescriptor(f1no,
            std::bind(&SelectorCallback::onFDStateChangeCallback,
                    selectorCallback.get(), _1, _2));
//    selector->addFileDescriptor(f2no,
//            (onFDStateChangeCallback) &SelectorCallback::onFDStateChangeCallback);

    fclose(f1);
    fclose(f2);
}
