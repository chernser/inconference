#include <memory>

//
// Created by Sergey Chernov on 2018-12-05.
//
#include <memory>

#include "../src/Mediator.h"


#include "gtest/gtest.h"
#include "../src/SharedBuffer.h"

using namespace Mediation;
using namespace Endpoints;
using namespace Memory;

auto bufferFactory = shared_ptr<FixedSizeBufferPool>(
        new FixedSizeBufferPool(DEFAULT_BUFFER_SIZE, 1, DEFAULT_BUFFER_SIZE * 100));


TEST(MediatorTest, addEndpoint_removeEndpoint) {
    auto mediator = std::make_unique<Mediator>(bufferFactory);

    auto endpoint2 = shared_ptr<Endpoint>(new Endpoint("endpoint2", true));
    {
        auto endpoint1 = shared_ptr<Endpoint>(new Endpoint("endpoint1", true));
        auto endpoint3 = shared_ptr<Endpoint>(new Endpoint("endpoint3", true));


        mediator->addEndpoint(endpoint1);
        mediator->addEndpoint(endpoint2);
        mediator->addEndpoint(endpoint3);
    }


    {
        GTEST_ASSERT_EQ(3, mediator->getEndpoints().size());
    }

    {
        mediator->removeEndpoint(endpoint2);
    }

    {
        GTEST_ASSERT_EQ(2, mediator->getEndpoints().size());
    }

}



