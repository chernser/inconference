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
        printf("fd %d triggered %d\n", fd, eventType);
    }


};

TEST(selecting_fd, libeven_singleThread_2_fds)
{
    auto selector = std::unique_ptr<LibEventFDSelector>(
            new LibEventFDSelector());
    auto selectorCallback = std::unique_ptr<SelectorCallback>(
            new SelectorCallback());

    auto f1 = fopen("./test_file_1", "w+");
    auto f2 = fopen("./test_file_2", "w+");
    printf("f1 %d\n", f1);
    auto f1no = fileno(f1);
    auto f2no = fileno(f2);

    printf("f1 no %d\n", f1no);

    selector->addFileDescriptor(f1no,
            std::bind(&SelectorCallback::onFDStateChangeCallback,
                    selectorCallback.get(), _1, _2));
//    selector->addFileDescriptor(f2no,
//            (onFDStateChangeCallback) &SelectorCallback::onFDStateChangeCallback);

    fwrite("test", 4, 1, f1);
    fflush(f1);

    selector->wakeUp();

    fclose(f1);
    fclose(f2);
}
