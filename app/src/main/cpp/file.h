//
// Created by qingyingliu on 2023/12/23.
//

#ifndef LOCALCDN_FILE_H
#define LOCALCDN_FILE_H

#include <string>
#include <mutex>
#include <unordered_map>
#include <fstream>
#include <filesystem>

namespace fs = std::__fs::filesystem;

class File {
private:
    // 文件名到互斥锁的映射。
    struct Lock {
        std::shared_ptr<std::mutex> mutex;
        int mutexInUseCount = 0;
    };
    static std::unordered_map<std::string, Lock> _lockMap;
    static std::mutex _mapMtx; // 保护lockMap的互斥锁。
    const std::string _path;

    std::shared_ptr<std::mutex> getMutex();

    std::string readWithThreadSafe();

    bool writeWithThreadSafe(const std::string &content);

public:
    File(const std::string &path);

    ~File();

    std::string read();

    bool write(const std::string &content);

    bool exists();

};


#endif //LOCALCDN_FILE_H
