//
// Created by Sergey Chernov on 2018-12-05.
//
#include <memory>

#include "../src/Mediator.h"


#include "gtest/gtest.h"
#include "../src/SharedBuffer.h"
#include "MediatorTestHelper.h"


using namespace Mediation;
using namespace Endpoints;
using namespace Memory;

auto bufferFactory = shared_ptr<FixedSizeBufferPool>(
        new FixedSizeBufferPool(DEFAULT_BUFFER_SIZE, 10, DEFAULT_BUFFER_SIZE * 100));


TEST(MediatorTest, addEndpoint_removeEndpoint) {
    auto mediator = std::unique_ptr<Mediator>(new Mediator(bufferFactory));

    auto endpoint2 = shared_ptr<Endpoint>(new Endpoint("endpoint2", true));
    {
        auto endpoint1 = shared_ptr<Endpoint>(new Endpoint("endpoint1", true));
        auto endpoint3 = shared_ptr<Endpoint>(new Endpoint("endpoint3", true));

        mediator->addEndpoint(endpoint1);
        mediator->addEndpoint(endpoint2);
        mediator->addEndpoint(endpoint3);
    }


    {
        GTEST_ASSERT_EQ(3, mediator->getNumberOfEndpoints());
    }

    {
        mediator->removeEndpoint(endpoint2);
    }

    {
        GTEST_ASSERT_EQ(2, mediator->getNumberOfEndpoints());
    }
}


TEST(MediatorTest, doIteration_localEndpoints) {
    auto mediator = std::unique_ptr<Mediator>(new Mediator(bufferFactory));

    auto endpoint1 = shared_ptr<SimpleTestEndpoint>(new SimpleTestEndpoint("ep1"));
    auto endpoint2 = shared_ptr<SimpleTestEndpoint>(new SimpleTestEndpoint("ep2"));
    auto endpoint3 = shared_ptr<SimpleTestEndpoint>(new SimpleTestEndpoint("ep3"));

    endpoint1->connect();
    endpoint2->connect();

    mediator->addEndpoint(endpoint1);
    mediator->addEndpoint(endpoint2);
    mediator->addEndpoint(endpoint3);

    mediator->doIteration();

    GTEST_ASSERT_EQ(2, mediator->getNumberOfEndpoints());

    uint32_t data_size = 5;
    auto test_data_1 = shared_ptr<uint8_t>(new uint8_t[data_size]{0xAE, 0xAA, 0x44, 0xAA, 0xBB});

    endpoint1->setReadyData(test_data_1, data_size);

    mediator->doIteration();

    GTEST_ASSERT_EQ(0, endpoint2->getReadBytes());

    mediator->doIteration();
    mediator->doIteration();

    GTEST_ASSERT_EQ(data_size, endpoint2->getReadBytes());
}
