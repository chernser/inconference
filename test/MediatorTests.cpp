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

auto bufferFactory = shared_ptr<BufferFactory>(new BufferFactory());


TEST(MediatorTest, addEndpoint_removeEndpoint) {
    auto mediator = std::make_unique<Mediator>(bufferFactory);

    auto endpoint2 = shared_ptr<Endpoint>(new Endpoint("endpoint2"));
    {
        auto endpoint1 = shared_ptr<Endpoint>(new Endpoint("endpoint1"));
        auto endpoint3 = shared_ptr<Endpoint>(new Endpoint("endpoint3"));


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



