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
#include <filesystem>
#include "utils.h"

namespace fs = std::__fs::filesystem;

class FilePath {
private:
    std::string path;
    std::mutex mtx;  // 互斥锁

public:
    FilePath(const std::string &filePath) : path(filePath) {}

    std::string read();


    void write(const std::string &content);

    void remove();

    std::string getPath() const {
        return this->path;
    }
};


#endif //JNITEST1_FILEPATH_H

