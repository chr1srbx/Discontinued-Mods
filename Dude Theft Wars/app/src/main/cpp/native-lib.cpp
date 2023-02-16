#include <EGL/egl.h> // need to make a common.h that contains all these headers cuz this is nasty
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
#include "http/cpr/cpr.h"
#include "Misc/Logging.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_android.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Obfuscation/Obfuscate.h"
#include <stdio.h>
#include <android/native_window_jni.h>
#include <jni.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include "Misc/JNIStuff.h"
#include "Misc/FileWrapper.h"
#include "Misc/Utils.h"
#include "ByNameModding/BNM.hpp"
#include "Obfuscation/Custom_Obfuscate.h"
#include "Misc/FunctionPointers.h"
#include "Hooking/Hooks.h"
#include "Misc/ImGuiStuff.h"
#include "Menu.h"
#include "Hooking/JNIHooks.h"
#include "Memory/MemoryPatch.h"
// the private version held by certain polarmodders has image loading and a lot more

EGLBoolean (*old_eglSwapBuffers)(...);
EGLBoolean new_eglSwapBuffers(EGLDisplay _display, EGLSurface _surface) {
    SetupImGui();
    Menu::DrawImGui();

    return old_eglSwapBuffers(_display, _surface);
}
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * reserved)
{
    jvm = vm;
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    UnityPlayer_cls = env->FindClass(OBFUSCATE("com/unity3d/player/UnityPlayer"));
    UnityPlayer_CurrentActivity_fid = env->GetStaticFieldID(UnityPlayer_cls,
                                                            OBFUSCATE("currentActivity"),
                                                            OBFUSCATE("Landroid/app/Activity;"));
    hook((void *) env->functions->RegisterNatives, (void *) hook_RegisterNatives,
         (void **) &old_RegisterNatives);

    return JNI_VERSION_1_6;
}

void Patches()
{

}

void *hack_thread(void *)
{
    using namespace BNM;
    do {
        sleep(1);
    } while (!Il2cppLoaded());
    AttachIl2Cpp();
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x44B3EC), (void*) Menu::MPPlayerController, (void**)&Menu::oldMPPlayerController);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x3B6FAC), (void*) Menu::PlayerController, (void**)&Menu::oldPlayerController);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x45C388), (void*) Menu::MPWeapon, (void**)&Menu::oldMPWeapon);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x4E7808), (void*) Menu::GameManager, (void**)&Menu::oldGameManager);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0xD056E0), (void*) Menu::Wheel, (void**)&Menu::oldWheel);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x3F7598), (void*) Menu::MPGameManager, (void**)&Menu::oldMPGameManager);
    DobbyHook((void*)getAbsoluteAddress("libil2cpp.so", 0x42DB58), (void*) Menu::Weapon, (void**)&Menu::oldWeapon);
    Patches();
    Menu::Screen_get_height = (int (*)()) OBFUSCATE_BYNAME_METHOD("UnityEngine", "Screen", "get_height",0);
    Menu::Screen_get_width = (int (*)()) OBFUSCATE_BYNAME_METHOD("UnityEngine", "Screen", "get_width", 0);
    Pointers::LoadPointers();
    DetachIl2Cpp();
    return NULL;
}
__attribute__((constructor))
void lib_main()
{
    auto eglhandle = dlopen(OBFUSCATE("libunity.so"), RTLD_LAZY);
    dlerror();
    auto eglSwapBuffers = dlsym(eglhandle, OBFUSCATE("eglSwapBuffers"));
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        LOGE(OBFUSCATE("Cannot load symbol 'eglSwapBuffers': %s"), dlsym_error);
    } else
    {
        hook(eglSwapBuffers, (void *) new_eglSwapBuffers, (void **) &old_eglSwapBuffers);
    }
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}