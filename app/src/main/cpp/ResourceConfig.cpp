//
// Created by qingyingliu on 2023/12/22.
//

#include "ResourceConfig.h"
ResourceConfig* ResourceConfig::GLOBAL_CONFIG = nullptr;

const std::string &ResourceConfig::getRootDir() const {
    return rootDir;
}
