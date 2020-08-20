#include <jni.h>
#include <string>
#include <sys/utsname.h>

void unameFunction(char* result) {

    utsname u_name;
    uname(&u_name);
    strcpy(result, u_name.release);
}

extern "C" JNIEXPORT jstring JNICALL
Java_me_zhj_device_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    char s[65];
    unameFunction(s);
    return env->NewStringUTF(s);
}

