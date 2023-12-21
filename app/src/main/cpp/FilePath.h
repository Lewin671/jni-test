//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_FILEPATH_H
#define JNITEST1_FILEPATH_H

#include <fstream>
#include <string>

#include <fstream>
#include <string>
#include <mutex>

class FilePath {
private:
    std::string path;
    std::mutex mtx;  // 互斥锁

public:
    FilePath(const std::string &filePath) : path(filePath) {}

    std::string read() {
        std::lock_guard<std::mutex> lock(mtx);  // 获取锁

        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + path);
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    void write(const std::string &content) {
        std::lock_guard<std::mutex> lock(mtx);  // 获取锁

        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + path);
        }

        file << content;
        file.close();
    }

    std::string getPath() const {
        return this->path;
    }
};


#endif //JNITEST1_FILEPATH_H

