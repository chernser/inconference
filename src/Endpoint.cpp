//
// Created by Sergey Chernov on 2018-12-05.
//

#include "Endpoint.h"


Endpoints::Endpoint::Endpoint(const std::string name, bool isLocal) : name(name), endpointState(),
                                                                      local(isLocal) {

}

const std::string &Endpoints::Endpoint::getName() const {
    return name;
}

const Endpoints::EndpointState &Endpoints::Endpoint::getEndpointState() const {
    return endpointState;
}

bool Endpoints::Endpoint::isLocal() const {
    return local;
}
