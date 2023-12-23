//
// Created by qingyingliu on 2023/12/22.
//

#include "FilePath.h"

void FilePath::remove() {
    fs::remove(path);
}

void FilePath::write(const std::string &content) {
    std::lock_guard<std::mutex> lock(mtx);  // 获取锁

    auto dirPathStr = fs::path(path).parent_path().string() + "/";

    utils::create_directories_if_not_exists(dirPathStr);

    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file111: " + path);
    }

    file << content;
    file.close();
}

std::string FilePath::read() {
    std::lock_guard<std::mutex> lock(mtx);  // 获取锁

    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
}
