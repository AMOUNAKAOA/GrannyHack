#include <jni.h>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "Menu.h"



void (*orig_BasicController_Update)(void* thisPtr); // 0x00802230
void h_BasicController_Update(void* thisPtr)
{
    void* instance = thisPtr;
    if (instance)
    {
        // speed hack, write value to adress
        float mySpeed = 200.0f;
        float* speedPtrA = (float*)((uintptr_t)instance + 0x2C);
        *speedPtrA = mySpeed;
    }
    orig_BasicController_Update(thisPtr);
}

void (*orig_Controller3DExample_Update)(void* thisptr); // 0x00777C0C
void h_Controller3DExample_Update(void* thisptr)
{
    void* instance = thisPtr;
    if (instance)
    {
        // speed hack, write value to adress
        float mySpeed = 200.0f;
        float* speedPtrA = (float*)((uintptr_t)instance + 0x18);
        *speedPtrA = mySpeed;
    }
    orig_Controller3DExample_Update(thisPtr);
}




void (*orig_resetPlayerMovement)(void* thisPtr);
void h_resetPlayerMovement(void* thisPtr)
{
    void* instance = thisPtr;
    if (instance)
    {
        // speed hack, write value to adress
        float mySpeed = 200.0f;
        float* speedPtrA = (float*)((uintptr_t)instance + 0x38);
        float* speedPtrB = (float*)((uintptr_t)instance + 0x3C);
        float* speedPtrC = (float*)((uintptr_t)instance + 0x40);
        float* speedPtrD = (float*)((uintptr_t)instance + 0x44);
        float* speedPtrE = (float*)((uintptr_t)instance + 0x48);
        *speedPtrA = mySpeed;
        *speedPtrB = mySpeed;
        *speedPtrC = mySpeed;
        *speedPtrD = mySpeed;
        *speedPtrE = mySpeed;
    }
    orig_resetPlayerMovement(thisPtr);
}





EGLBoolean (*old_eglSwapBuffers)(...);
EGLBoolean new_eglSwapBuffers(EGLDisplay _display, EGLSurface _surface) {
    SetupImGui();
    Menu::DrawImGui();

    return old_eglSwapBuffers(_display, _surface);
}




jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = nullptr;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    // 🔥 ton code d'initialisation ici
    // ex: logs, init, hooks, etc.

    return JNI_VERSION_1_6;
}

void *hack_thread(void *)
{
    do {
        sleep(1);
    } while (!isLibraryLoaded("libil2cpp.so"));
    hook(getAbsoluteAddress("libil2cpp.so", 0x00852AD4), (void*) h_resetPlayerMovement, (void**) &orig_resetPlayerMovement);
    hook(getAbsoluteAddress("libil2cpp.so", 0x00802230), (void*) h_BasicController_Update, (void**) &orig_BasicController_Update);
    hook(getAbsoluteAddress("libil2cpp.so", 0x00777C0C), (void*) h_Controller3DExample_Update, (void**) &orig_Controller3DExample_Update);
    return NULL;
}


__attribute__((constructor))
void lib_main()
{
    dlerror(); 
    auto eglhandle = dlopen("libEGL.so", RTLD_LAZY);
    const char *dlopen_error = dlerror();
    if (!eglhandle || dlopen_error)
    {
        eglhandle = dlopen("libunity.so", RTLD_LAZY); // I have no idea if this works it was just to me that it would fix crashes so I did it really quickly
        dlopen_error = dlerror();

        if (!eglhandle || dlopen_error)
        {
            printf("libunity.so failed");
        }
    }
    auto eglSwapBuffers = dlsym(eglhandle, "eglSwapBuffers");

    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        printf("Cannot load symbol 'eglSwapBuffers': %s", dlsym_error);
    } else
    {
        hook(eglSwapBuffers, (void *) new_eglSwapBuffers, (void **) &old_eglSwapBuffers);
    }

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}