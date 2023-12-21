//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_RESOURCECONFIG_H
#define JNITEST1_RESOURCECONFIG_H

#include "FilePath.h"
#include "ResourceItem.h"
#include "ThreadPool.h"
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include "utils.h"
#include "network.h"

class ResourceConfig {
private:
    // 总的配置文件
    std::string rootDir;
    std::string configPath;
    std::unordered_map<std::string, std::shared_ptr<ResourceItem>> resourceItems;
    ThreadPool threadPool{5};
    static ResourceConfig *GLOBAL_CONFIG;

    bool endsWith(const std::string &str, const std::string &suffix) {
        if (str.size() < suffix.size())
            return false;
        return str.rfind(suffix) == (str.size() - suffix.size());
    }

    ResourceConfig(std::string &rootDir) {
        this->rootDir = rootDir;
        if (endsWith(rootDir, "/")) {
            this->configPath = rootDir + "config";
        } else {
            this->configPath = rootDir + "/config";
        }

        loadThis();
    }

public:

    static void setUp(std::string &rootDir) {
        GLOBAL_CONFIG = new ResourceConfig(rootDir);
        std::shared_ptr<Request> request = std::make_shared<Request>();
        request->url = "https://www.baidu.com/";

        auto response = Network::sendRequest(request);
        utils::log(response->statusCode);
        utils::log(response->data);
    }

    static ResourceConfig *getInstance() {
        return GLOBAL_CONFIG;
    }


    std::shared_ptr<ResourceItem> getResourceItem(std::string &url) {
        std::string urlWithoutPrefix = utils::removePrefix(url);
        auto it = resourceItems.find(urlWithoutPrefix);
        if (it != resourceItems.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    void putResourceItem(std::string &url, std::shared_ptr<ResourceItem> resourceItem) {
        std::string urlWithoutPrefix = utils::removePrefix(url);
        resourceItems[urlWithoutPrefix] = resourceItem;
        threadPool.enqueue([=] {
            serializeThis();
        });
    }

    void serializeThis() {
        std::ofstream file(configPath);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + configPath);
        }

        for (const auto &item: resourceItems) {
            file << item.first << "\n";  // 写入URL
            file << item.second->getOriginalUrl() << "\n";
            file << item.second->getPackageDir() << "\n";
        }

        file.close();
    }

    void loadThis() {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            return;
        }

        std::string url;
        while (std::getline(file, url)) {
            std::string originalUrl;
            std::string packageDir;
            if (std::getline(file, originalUrl) && std::getline(file, packageDir)) {
                auto resourceItem = std::make_shared<ResourceItem>(packageDir, originalUrl);
                resourceItems[url] = resourceItem;
            }
        }

        file.close();
    }

};


#endif //JNITEST1_RESOURCECONFIG_H
