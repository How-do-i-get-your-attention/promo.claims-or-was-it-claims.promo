#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_how_1do_1i_1get_1your_1attentions_android_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}