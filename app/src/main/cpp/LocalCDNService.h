//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_LOCALCDNSERVICE_H
#define JNITEST1_LOCALCDNSERVICE_H

#include "network.h"
#include "disk-cache.h"

class LocalCDNService {

public:
    static std::shared_ptr<Response> sendRequest(const std::shared_ptr<Request> request);
};

#endif //JNITEST1_LOCALCDNSERVICE_H
