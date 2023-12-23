//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_NETWORK_H
#define JNITEST1_NETWORK_H

#include "utils.h"
#include <string>
#include <unordered_map>

#include "jvm.h"
#include <jni.h>

struct Request {
    Request(const std::string &url, const std::unordered_map<std::string, std::string> &headers);

    std::string url;
    std::unordered_map<std::string, std::string> headers;
};

struct Response {
    std::string statusCode;
    std::unordered_map<std::string, std::string> headers;
    std::string data;

    Response(const std::string &statusCode,
             const std::unordered_map<std::string, std::string> &headers, const std::string &data);
};

class Network {
public:
    static std::shared_ptr<Response> sendRequest(const std::shared_ptr<Request> requestProxy);
};


#endif //JNITEST1_NETWORK_H
