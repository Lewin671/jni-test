//
// Created by qingyingliu on 2023/12/23.
//

#ifndef LOCALCDN_DISK_CACHE_H
#define LOCALCDN_DISK_CACHE_H

#include <string>
#include "utils.h"
#include "ResourceConfig.h"
#include "file.h"
#include "network.h"

class DiskCache {
private:
    static std::string getTargetPath(const std::string &packageName, const std::string &url);

public:
    static std::string
    read(const std::string &url, const std::string &packageName, const std::string &packageVersion);

    static bool
    write(const std::string &url, const std::string &packageName, const std::string &packageVersion,
          const std::string &content);
};


#endif //LOCALCDN_DISK_CACHE_H
