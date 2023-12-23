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

void utils::log(const std::string &msg) {
    __android_log_print(ANDROID_LOG_ERROR, "myTag", "%s", msg.c_str());
}

std::string utils::convertJStringToString(JNIEnv *env, jstring jStr) {
    if (jStr == NULL)
        return std::string{};

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes,
                                                                       env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte *pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *) pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;

}

std::unordered_map<std::string, std::string>
utils::convertJObjectToUnorderedMap(JNIEnv *env, jobject jMap) {
    if (jMap == NULL) {
        return std::unordered_map<std::string, std::string>();
    }
    std::unordered_map<std::string, std::string> map;

    jclass mapClass = env->GetObjectClass(jMap);
    jmethodID entrySet = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject set = env->CallObjectMethod(jMap, entrySet);

    jclass setClass = env->GetObjectClass(set);
    jmethodID toArray = env->GetMethodID(setClass, "toArray", "()[Ljava/lang/Object;");
    jobjectArray array = (jobjectArray) env->CallObjectMethod(set, toArray);

    int arraySize = env->GetArrayLength(array);
    for (int i = 0; i < arraySize; i++) {
        jobject entry = env->GetObjectArrayElement(array, i);

        jclass entryClass = env->GetObjectClass(entry);
        jmethodID getKey = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
        jmethodID getValue = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

        jstring jKey = (jstring) env->CallObjectMethod(entry, getKey);
        jstring jValue = (jstring) env->CallObjectMethod(entry, getValue);

        auto key = convertJStringToString(env, jKey);
        auto value = convertJStringToString(env, jValue);
        map[key] = value;

        env->DeleteLocalRef(entry);
        env->DeleteLocalRef(jKey);
        env->DeleteLocalRef(jValue);
    }

    env->DeleteLocalRef(array);
    env->DeleteLocalRef(setClass);
    env->DeleteLocalRef(set);
    env->DeleteLocalRef(mapClass);
    return map;
}

jstring utils::convertStringToJString(JNIEnv *env, const std::string &str) {
    return env->NewStringUTF(str.c_str());
}

jobject utils::convertUnorderedMapToJObject(JNIEnv *env,
                                            const std::unordered_map<std::string, std::string> &map) {
    jclass mapClass = env->FindClass("java/util/HashMap");
    jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
    jobject jmap = env->NewObject(mapClass, mapConstructor, NULL);

    jmethodID put = env->GetMethodID(mapClass, "put",
                                     "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    for (const auto &pair: map) {
        jstring jkey = convertStringToJString(env, pair.first);
        jstring jvalue = convertStringToJString(env, pair.second);

        env->CallObjectMethod(jmap, put, jkey, jvalue);

        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(jvalue);
    }

    return jmap;
}

jbyteArray utils::convertStringToJByteArray(JNIEnv *env, const std::string &str) {
    jbyteArray byteArray = env->NewByteArray(str.length());
    env->SetByteArrayRegion(byteArray, 0, str.length(),
                            reinterpret_cast<const jbyte *>(str.c_str()));
    return byteArray;
}

bool utils::create_directories_if_not_exists(const std::string &path) {
    fs::path dir(path);
    if (fs::exists(dir)) {
        return true;
    }
    if (!fs::create_directories(dir)) {
        utils::log("create dir failed: " + path);
        return false;
    }
    return true;
}


std::string utils::convertJByteArrayToString(JNIEnv *env, jbyteArray byteArray) {
    jbyte *bytes = env->GetByteArrayElements(byteArray, nullptr);
    jsize length = env->GetArrayLength(byteArray);
    std::string result(reinterpret_cast<char *>(bytes), length);
    env->ReleaseByteArrayElements(byteArray, bytes, JNI_ABORT);
    return result;
}
