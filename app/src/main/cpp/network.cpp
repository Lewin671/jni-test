//
// Created by qingyingliu on 2023/12/22.
//

#include "network.h"


std::shared_ptr<Response> Network::sendRequest(const std::shared_ptr<Request> requestProxy) {
    JNIEnv *env;
    jint result = JVM::jvmInstance->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (result != JNI_OK) {
        //todo
        return nullptr;
        // 处理错误
    }

    // 假设你已经有了一个有效的 JNIEnv* env 和 jobject instance
    const char *className = "com/example/jnitest1/Network";  // 类名

    jclass networkClass = env->FindClass(className);

    if (networkClass == NULL) {
        // 处理错误
        return nullptr;
    }

    jmethodID sendRequestMethod = env->GetStaticMethodID(networkClass, "sendRequest",
                                                         "(Lcom/example/jnitest1/Request;)Lcom/example/jnitest1/Response;");
    if (sendRequestMethod == NULL) {
        // 处理错误
        return nullptr;
    }

    jclass requestClass = env->FindClass("com/example/jnitest1/Request");
    if (requestClass == NULL) {
        // 处理错误
        return nullptr;
    }

    jmethodID requestConstructor = env->GetMethodID(requestClass, "<init>",
                                                    "(Ljava/lang/String;Ljava/util/Map;)V");
    if (requestConstructor == NULL) {
        // 处理错误
        return nullptr;
    }

    jstring url = env->NewStringUTF(requestProxy->url.c_str());
    // 获取 HashMap 类和构造函数
    jclass hashMapClass = env->FindClass("java/util/HashMap");
    jmethodID hashMapConstructor = env->GetMethodID(hashMapClass, "<init>", "()V");

// 创建一个新的 HashMap 对象
    jobject headers = env->NewObject(hashMapClass, hashMapConstructor);

// 获取 HashMap.put 方法
    jmethodID putMethod = env->GetMethodID(hashMapClass, "put",
                                           "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    // 添加键值对
    jstring key = env->NewStringUTF("Content-Type");
    jstring value = env->NewStringUTF("application/json");
    env->CallObjectMethod(headers, putMethod, key, value);

    jobject request = env->NewObject(requestClass, requestConstructor, url,
                                     headers);  // 假设 headers 为 null

    jobject response = env->CallStaticObjectMethod(networkClass, sendRequestMethod, request);

    // 获取 Response 类和它的方法
    jclass responseClass = env->FindClass("com/example/jnitest1/Response");
    jmethodID getStatusCodeMethod = env->GetMethodID(responseClass, "getStatusCode",
                                                     "()Ljava/lang/String;");
    jmethodID getHeadersMethod = env->GetMethodID(responseClass, "getHeaders", "()Ljava/util/Map;");
    jmethodID getBytesMethod = env->GetMethodID(responseClass, "getBytes", "()[B");

// 调用方法并获取返回值
    jstring statusCode = (jstring) env->CallObjectMethod(response, getStatusCodeMethod);
    jobject responseHeader = env->CallObjectMethod(response, getHeadersMethod);
    jbyteArray bytes = (jbyteArray) env->CallObjectMethod(response, getBytesMethod);

// 将 jstring 转换为 std::string
    const char *statusCodeChars = env->GetStringUTFChars(statusCode, 0);
    std::string statusCodeStr(statusCodeChars);
    env->ReleaseStringUTFChars(statusCode, statusCodeChars);

// 将 Map 转换为 std::unordered_map
    std::unordered_map<std::string, std::string> headersMap;
    jclass mapClass = env->FindClass("java/util/Map");
    jmethodID entrySetMethod = env->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");
    jobject entrySet = env->CallObjectMethod(responseHeader, entrySetMethod);
    jclass setClass = env->FindClass("java/util/Set");
    jmethodID iteratorMethod = env->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
    jobject iterator = env->CallObjectMethod(entrySet, iteratorMethod);
    jclass iteratorClass = env->FindClass("java/util/Iterator");
    jmethodID hasNextMethod = env->GetMethodID(iteratorClass, "hasNext", "()Z");
    jmethodID nextMethod = env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");
    while (env->CallBooleanMethod(iterator, hasNextMethod)) {
        jobject entry = env->CallObjectMethod(iterator, nextMethod);
        jclass entryClass = env->FindClass("java/util/Map$Entry");
        jmethodID getKeyMethod = env->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
        jmethodID getValueMethod = env->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");
        jstring key = (jstring) env->CallObjectMethod(entry, getKeyMethod);
        jstring value = (jstring) env->CallObjectMethod(entry, getValueMethod);
        const char *keyChars = env->GetStringUTFChars(key, 0);
        const char *valueChars = env->GetStringUTFChars(value, 0);
        headersMap[keyChars] = valueChars;
        env->ReleaseStringUTFChars(key, keyChars);
        env->ReleaseStringUTFChars(value, valueChars);
    }

// 将 jbyteArray 转换为 std::string
    jsize length = env->GetArrayLength(bytes);
    jbyte *bytesElements = env->GetByteArrayElements(bytes, 0);
    std::string data((char *) bytesElements, length);
    env->ReleaseByteArrayElements(bytes, bytesElements, 0);

    // 现在你可以使用 statusCodeStr、headersMap 和 data 了

    auto responseProxy = std::make_shared<Response>();
    responseProxy->data = data;
    responseProxy->headers = headersMap;
    responseProxy->statusCode = statusCodeStr;
    return responseProxy;
}
