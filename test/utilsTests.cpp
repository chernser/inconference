#include <memory>
#include <unistd.h>

#include "../src/utils.h"

#include "gtest/gtest.h"

// using namespace Memory;
using namespace LibUtils;

TEST(basic_task_destributer, single_thread)
{

    BasicTaskDistributor<uint16_t> distributor(2, 3);
    uint16_t tasks[5] = {100, 300, 200, 400, 500};

    for (uint i = 0; i < sizeof(tasks) / sizeof(tasks[0]); i++)
    {
        distributor.submitTask(tasks[i]);
    }


    ASSERT_EQ(tasks[0], distributor.pullNextTask(0).task);
    ASSERT_EQ(tasks[2], distributor.pullNextTask(0).task);
    ASSERT_EQ(tasks[4], distributor.pullNextTask(0).task);
    ASSERT_FALSE(distributor.pullNextTask(0).hasValue);
}