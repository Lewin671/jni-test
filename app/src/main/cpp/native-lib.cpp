#include <jni.h>
#include <string>
#include "FilePath.h"
#include "ResourceConfig.h"
#include "jvm.h"

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
    const char *rootDirCStr = env->GetStringUTFChars(root_dir, 0);
    std::string rootDirStr(rootDirCStr);
    env->ReleaseStringUTFChars(root_dir, rootDirCStr);
    ResourceConfig::setUp(rootDirStr);
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JVM::jvmInstance = vm;
    return JNI_VERSION_1_6; // Or the desired JNI version
}

