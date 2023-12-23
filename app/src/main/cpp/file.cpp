//
// Created by qingyingliu on 2023/12/23.
//

#include "file.h"
#include "utils.h"

std::unordered_map<std::string, File::Lock> File::_lockMap;
std::mutex  File::_mapMtx; // 保护lockMap的互斥锁。
File::File(const std::string &path) : _path(path) {
    {
//        std::lock_guard<std::mutex> lockGuardForMap(File::_mapMtx); // 锁定mapMtx。

        auto it = _lockMap.find(_path);
        if (it != _lockMap.end()) {
            it->second.mutexInUseCount++;
        } else {
            const std::shared_ptr<std::mutex> &mtx = std::make_shared<std::mutex>();
            Lock lock{};
            lock.mutex = mtx;
            lock.mutexInUseCount++;
            _lockMap[path] = lock;
        }
    }
}

std::string File::read() {
    const std::shared_ptr<std::mutex> &mutex = getMutex();
    if (mutex == nullptr) {
        throw std::runtime_error("mutex shouldn't be null");
    }
    std::lock_guard<std::mutex> lock(*mutex);
    return readWithThreadSafe();
}


bool File::write(const std::string &content) {
    const std::shared_ptr<std::mutex> &mutex = getMutex();
    if (mutex == nullptr) {
        throw std::runtime_error("mutex shouldn't be null");
    }
    std::lock_guard<std::mutex> lock(*mutex);
    return writeWithThreadSafe(content);
}

File::~File() {
    {
        std::lock_guard<std::mutex> lockGuardForMap(_mapMtx); // 锁定mapMtx。

        auto it = _lockMap.find(_path);
        if (it != _lockMap.end()) {
            it->second.mutexInUseCount--;
            if (it->second.mutexInUseCount == 0) {
                _lockMap.erase(_path);
            }
        }
    }
}

std::shared_ptr<std::mutex> File::getMutex() {
    auto it = _lockMap.find(_path);
    if (it != _lockMap.end()) {
        return it->second.mutex;
    }

    return nullptr;
}

std::string File::readWithThreadSafe() {
    std::ifstream file(_path);
    if (!file.is_open()) {
        return std::string{};
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
}

bool File::writeWithThreadSafe(const std::string &content) {
    // Extract the directory path from the file path
    fs::path directory = fs::path(_path).parent_path();

    // Check if the directory exists, and create it if not
    if (!fs::exists(directory)) {
        if (!fs::create_directories(directory)) {
            return false;
        }
    }

    std::ofstream file(_path);
    if (!file.is_open()) {
        return false;
    }

    file << content;
    file.close();
    return true;
}

bool File::exists() {
    return fs::exists(_path);
}



