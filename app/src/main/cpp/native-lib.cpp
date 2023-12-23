#include <jni.h>
#include <string>
#include "FilePath.h"
#include "ResourceConfig.h"
#include "jvm.h"
#include "LocalCDNService.h"

int i = 0;

extern "C" JNIEXPORT jstring
JNICALL
Java_com_example_jnitest1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    FilePath *filePath = new FilePath("/data/data/com.example.jnitest1/testfile.txt");
    std::string hello = filePath->read();
    hello.append(std::to_string(i++)).append(",");
    filePath->write(hello);

    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jnitest1_MainActivity_setUpStorage(JNIEnv *env, jobject thiz, jstring root_dir) {
    auto rootDirStr{utils::convertJStringToString(env, root_dir)};
    ResourceConfig::setUp(rootDirStr);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jnitest1_LocalCDNService_sendRequest(JNIEnv *env, jclass cls, jobject jRequest) {
    try {
        if (jRequest == NULL) {
            return NULL;
        }

        // 获取Request类和其方法
        jclass requestClass = env->GetObjectClass(jRequest);
        jmethodID getUrlMethod = env->GetMethodID(requestClass, "getUrl", "()Ljava/lang/String;");
        jmethodID getHeadersMethod = env->GetMethodID(requestClass, "getHeaders",
                                                      "()Ljava/util/Map;");

        // 调用方法获取url和headers
        jstring jUrl = (jstring) env->CallObjectMethod(jRequest, getUrlMethod);
        if (jUrl == nullptr) {
            return nullptr;
        }

        jobject jHeaders = env->CallObjectMethod(jRequest, getHeadersMethod);

        auto request = std::make_shared<Request>(utils::convertJStringToString(env, jUrl),
                                                 utils::convertJObjectToUnorderedMap(env,
                                                                                     jHeaders));
        auto response = LocalCDNService::sendRequest(request);
        if (response != nullptr) {
            jclass responseClass = env->FindClass(
                    "com/example/jnitest1/Response"); // 请替换为你的Response类名
            jmethodID constructor = env->GetMethodID(responseClass, "<init>",
                                                     "(Ljava/util/Map;Ljava/lang/String;[B)V");
            jobject jResponse = env->NewObject(responseClass, constructor,
                                               utils::convertUnorderedMapToJObject(env,
                                                                                   response->headers),
                                               utils::convertStringToJString(env,
                                                                             response->statusCode),
                                               utils::convertStringToJByteArray(env,
                                                                                response->data));
            return jResponse;
        }
    } catch (std::exception e) {
        utils::log("sendRequest error " + std::string{e.what()});
    }
    return nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JVM::jvmInstance = vm;
    return JNI_VERSION_1_6; // Or the desired JNI version
}

