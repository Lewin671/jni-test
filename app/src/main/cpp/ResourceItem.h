//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_RESOURCEITEM_H
#define JNITEST1_RESOURCEITEM_H

#include "FilePath.h"

class ResourceItem {
private:
    std::unique_ptr<FilePath> filePath;
    std::string originalUrl;
    std::string packageDir;

private:

    std::string urlToFilename(const std::string &url) {
        // 使用std::hash生成url的哈希值
        std::hash<std::string> hasher;
        size_t hash = hasher(url);

        // 将哈希值转换为字符串
        std::string filename = std::to_string(hash);

        return filename;
    }

public:
    // 如 /data/data/com.test.com/files/cache/
    ResourceItem(const std::string &packageDir, const std::string &url) {
        //todo: 转换成本地文件路径
        this->packageDir = packageDir;
        std::string fileName = urlToFilename(url);
        this->filePath = std::make_unique<FilePath>(packageDir + fileName);
        this->originalUrl = url;
    }

    std::string read() const {
        if (filePath) {
            return filePath->read();
        }
    }

    void write(const std::string &content) {
        if (filePath) {
            filePath->write(content);
        }
    }


    const std::string getPackageDir() const {
        return packageDir;
    }

    const std::string getOriginalUrl() const {
        return originalUrl;
    }

};

#endif //JNITEST1_RESOURCEITEM_H

