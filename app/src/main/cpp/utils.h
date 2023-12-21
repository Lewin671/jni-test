//
// Created by qingyingliu on 2023/12/22.
//

#ifndef JNITEST1_UTILS_H
#define JNITEST1_UTILS_H


#include <string>
#include "android/log.h"

class utils {
public:
    static std::string removePrefix(std::string &url);

    static void log(std::string &msg);
};


#endif //JNITEST1_UTILS_H
