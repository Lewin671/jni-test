//
// Created by qingyingliu on 2023/12/22.
//

#include "network.h"


std::shared_ptr<Response> Network::sendRequest(const std::shared_ptr<Request> requestProxy) {
    JNIEnv *env;
    jint result = JVM::jvmInstance->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (result != JNI_OK) {
        return nullptr;
    }

    // 假设你已经有了一个有效的 JNIEnv* env 和 jobject instance
    const char *className = "com/example/jnitest1/Network";  // 类名

    jclass networkClass = env->FindClass(className);

    if (networkClass == NULL) {
        return nullptr;
    }

    jmethodID sendRequestMethod = env->GetStaticMethodID(networkClass, "sendRequest",
                                                         "(Lcom/example/jnitest1/Request;)Lcom/example/jnitest1/Response;");
    if (sendRequestMethod == NULL) {
        return nullptr;
    }

    jclass requestClass = env->FindClass("com/example/jnitest1/Request");
    if (requestClass == NULL) {
        return nullptr;
    }

    jmethodID requestConstructor = env->GetMethodID(requestClass, "<init>",
                                                    "(Ljava/lang/String;Ljava/util/Map;)V");
    if (requestConstructor == NULL) {
        return nullptr;
    }

    jstring jUrl = utils::convertStringToJString(env, requestProxy->url);
    jobject javaRequestHeaders = utils::convertUnorderedMapToJObject(env, requestProxy->headers);

    jobject jRequest = env->NewObject(requestClass, requestConstructor, jUrl,
                                      javaRequestHeaders);

    jobject jResponse;
    try {
        jResponse = env->CallStaticObjectMethod(networkClass, sendRequestMethod, jRequest);
    } catch (std::exception e) {
        utils::log("network exception: " + std::string{e.what()});
    }

    env->DeleteLocalRef(jUrl);
    env->DeleteLocalRef(javaRequestHeaders);
    env->DeleteLocalRef(jRequest);
    if (jResponse == nullptr) {
        return nullptr;
    }

    // 获取 Response 类和它的方法
    jclass responseClass = env->FindClass("com/example/jnitest1/Response");
    jmethodID getStatusCodeMethod = env->GetMethodID(responseClass, "getStatusCode",
                                                     "()Ljava/lang/String;");
    jmethodID getHeadersMethod = env->GetMethodID(responseClass, "getHeaders", "()Ljava/util/Map;");
    jmethodID getBytesMethod = env->GetMethodID(responseClass, "getBytes", "()[B");

    jstring jStatusCode = (jstring) env->CallObjectMethod(jResponse, getStatusCodeMethod);
    jobject jResponseHeader = env->CallObjectMethod(jResponse, getHeadersMethod);
    jbyteArray jBytes = (jbyteArray) env->CallObjectMethod(jResponse, getBytesMethod);

    auto statusCode = utils::convertJStringToString(env, jStatusCode);
    auto responseHeader = utils::convertJObjectToUnorderedMap(env, jResponseHeader);
    auto data = utils::convertJByteArrayToString(env, jBytes);

    env->DeleteLocalRef(jStatusCode);
    env->DeleteLocalRef(jResponseHeader);
    env->DeleteLocalRef(jBytes);

    auto responseProxy = std::make_shared<Response>(
            statusCode,responseHeader, data);
    return responseProxy;
}

Response::Response(const std::string &statusCode,
                   const std::unordered_map<std::string, std::string> &headers,
                   const std::string &data) : statusCode(statusCode), headers(headers),
                                              data(data) {}

Request::Request(const std::string &url,
                 const std::unordered_map<std::string, std::string> &headers) : url(url),
                                                                                headers(headers) {}
