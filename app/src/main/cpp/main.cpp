
#include <jni.h>
#include <fcntl.h>
#include "android/log.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string>
#include <cstdio>
#include <sys/utsname.h>
#include <dobby.h>

void entry();

void
__attribute__((constructor)) funcBeforeMain() {
    entry();
}


long get_module_base(const char *module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char line[1024];

    fp = fopen("/proc/self/maps", "r");

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                break;
            }
        }

        fclose(fp);
    }

    return addr;
}

namespace ZZArt {

    int z_uname(struct utsname* __buf);

    int (*p_uname)(struct utsname* __buf) = nullptr;

}

void entry() {

    auto lib_handle = dlopen("libc.so", RTLD_NOW);

    void * uname_fun = dlsym(lib_handle, "uname");

    long addr = get_module_base("libnative-lib.so");

    if (addr > 0) {
        DobbyHook()
    }

    if (uname_fun) {

        DobbyHook(uname_fun, (void *) ZZArt::z_uname,
                  (void **) &ZZArt::p_uname);

    };

    dlclose(lib_handle);
}

int ::ZZArt::z_uname(struct utsname *__buf) {
    std::string ver = "Dobby-linux";

    char release_char[56];
    strncpy(release_char, ver.c_str(), ver.length() + 1);

    int rel = p_uname(__buf);

    for(int i=0; i<56 ;i++)
    {
        (__buf)->release[i]=release_char[i];
    }

    return rel;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_EVERSION;
    } //if

    return JNI_VERSION_1_6;
}
