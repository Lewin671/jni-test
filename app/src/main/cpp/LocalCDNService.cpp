//
// Created by qingyingliu on 2023/12/22.
//

#include "LocalCDNService.h"
#include "ResourceConfig.h"
#include <unordered_map>

std::string packageName = "app-001";
std::string packageVersion = "001";

std::shared_ptr<Response> LocalCDNService::sendRequest(const std::shared_ptr<Request> request) {
    auto diskContent = DiskCache::read(request->url, packageName, packageVersion);
    if (!diskContent.empty()) {
        std::unordered_map<std::string, std::string> headers;
        headers["fetchType"] = "disk";
        return std::make_shared<Response>("200", headers, diskContent);
    }

    auto networkResponse = Network::sendRequest(request);
    if (networkResponse == nullptr) {
        return nullptr;
    }

    if (networkResponse->statusCode == "200") {
        networkResponse->headers["fetchType"] = "network";
        DiskCache::write(request->url, packageName, packageVersion, networkResponse->data);
        return networkResponse;
    } else {
        utils::log("unexpected status code: " + networkResponse->statusCode);
    }
    return nullptr;
}

std::shared_ptr<Response> sendRequest1(const std::shared_ptr<Request> request) {
    if (ResourceConfig::getInstance()->getRootDir().empty()) {
        return nullptr;
    }
    const std::shared_ptr<ResourceItem> &resourceItem = ResourceConfig::getInstance()->getResourceItem(
            request->url);
    if (resourceItem == nullptr) {
        const std::shared_ptr<ResourceItem> &newResourceItem = std::make_shared<ResourceItem>(
                ResourceConfig::getInstance()->getRootDir() + "data/", request->url);
        const std::shared_ptr<Response> &response = Network::sendRequest(request);
        if (response != nullptr) {
            if (response->statusCode == "200") {
                response->headers["fetchType"] = "network";
                newResourceItem->write(response->data);
                ResourceConfig::getInstance()->putResourceItem(request->url, newResourceItem);
            } else {
                utils::log("unexpected status code: " + response->statusCode);
            }
        }
        return response;
    } else {
        const std::string &content = resourceItem->read();
        if (!content.empty()) {
            std::unordered_map<std::string, std::string> headers;
            headers["fetchType"] = "disk";
            return std::make_shared<Response>("200", headers, content);
        } else {
            utils::log("resourceItem is invalid, remove and re-download.");
            resourceItem->remove();
            ResourceConfig::getInstance()->removeResourceItem(request->url);
        }

    }
    return nullptr;
}
