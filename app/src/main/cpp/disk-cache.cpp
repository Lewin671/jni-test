//
// Created by qingyingliu on 2023/12/23.
//

#include "disk-cache.h"

std::string DiskCache::read(const std::string &url, const std::string &packageName,
                            const std::string &packageVersion) {
    if (url.empty() || packageName.empty()) {
        return std::string{};
    }

    std::string filePath = getTargetPath(packageName, url);

    std::unique_ptr<File> file = std::make_unique<File>(filePath);
    if (file->exists()) {
        auto content = file->read();
        return content;
    }

    return std::string{};
}

std::string DiskCache::getTargetPath(const std::string &packageName, const std::string &url) {
    std::hash<std::string> hasher;
    std::string fileName = std::to_string(hasher(url));

    std::string dirName = utils::makePath(ResourceConfig::getInstance()->getRootDir(), packageName);
    return utils::makePath(dirName, fileName);
}

bool DiskCache::write(const std::string &url, const std::string &packageName,
                      const std::string &packageVersion, const std::string &content) {
    if (url.empty() || packageName.empty() || content.empty()) {
        return false;
    }

    std::string filePath = getTargetPath(packageName, url);

    std::unique_ptr<File> file = std::make_unique<File>(filePath);
    return file->write(content);
}
