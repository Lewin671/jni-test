//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_UTILS_H
#define JNITEST1_UTILS_H


#include <string>
#include <jni.h>
#include "android/log.h"
#include <unordered_map>
#include <filesystem>
#include <chrono>

namespace fs = std::__fs::filesystem;

class utils {
public:
    static std::string removePrefix(std::string &url);

    static void log(const std::string &msg);

    static std::string convertJStringToString(JNIEnv *env, jstring jStr);

    static std::unordered_map<std::string, std::string>
    convertJObjectToUnorderedMap(JNIEnv *env, jobject jMap);

    static jstring convertStringToJString(JNIEnv *env, const std::string &str);

    static jobject convertUnorderedMapToJObject(JNIEnv *env,
                                                const std::unordered_map<std::string, std::string> &map);

    static jbyteArray convertStringToJByteArray(JNIEnv *env, const std::string &str);

    static bool create_directories_if_not_exists(const std::string &path);

    static std::string convertJByteArrayToString(JNIEnv *env, jbyteArray byteArray);

    static long getCurrentTimeMilliSeconds();

    static std::string makePath(const std::string &baseDir, const std::string &fileName);

};


#endif //JNITEST1_UTILS_H
