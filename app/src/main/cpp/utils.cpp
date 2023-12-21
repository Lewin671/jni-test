//
// Created by qingyingliu on 2023/12/22.
//

#include "utils.h"

std::string utils::removePrefix(std::string &url) {
    std::string::size_type pos = url.find("://");
    if (pos != std::string::npos) {
        // pos + 3 是因为 "://" 是3个字符
        return url.substr(pos + 3);
    } else {
        return url;
    }
}

void utils::log(std::string &msg) {
    __android_log_print(ANDROID_LOG_ERROR, "myTag", "%s", msg.c_str());
}
