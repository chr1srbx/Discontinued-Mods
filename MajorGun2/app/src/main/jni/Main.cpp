#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <Unity/Vector3.hpp>
#include <GLES2/gl2.h>
#include <And64InlineHook/And64InlineHook.hpp>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu/Setup.h"
#include "Includes/Macros.h"
#include "Includes/Mono.h"
#include "Includes/Random.h"
//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")


bool health, developer, ammo, explosiontime, damage, freeshop, radius, xp, paused, noshield, grenades, medkits, nax;
float fireRate, killall, gravity;

void Pointers() {

}


void(*oldPlayerHealth)(void* obj);
void PlayerHealth(void* obj){
    if(obj != nullptr){
        if(health){
            *(float*)((uint64_t) obj + 0x24) = 99999;
            *(float*)((uint64_t) obj + 0x28) = 99999;
        }
    }
    oldPlayerHealth(obj);
}

void(*oldPlayerWeapon)(void* obj);
void PlayerWeapon(void* obj){
    if(obj != nullptr){
        if(ammo){
            *(bool*)((uint64_t) obj + 0xEC) = true;
        }

        void* CameraRecoil = *(void**)((uint64_t) obj + 0x80);
        if(CameraRecoil != nullptr){
            if(fireRate != NULL){
                *(float*)((uint64_t) CameraRecoil + 0x1C) = fireRate;
            }
        }
    }
    oldPlayerWeapon(obj);
}

void(*oldEnemy)(void* obj);
void Enemy(void* obj){
    if(obj != nullptr){
        if(damage){
            *(float*)((uint64_t) obj + 0xA4) = 1;
        }

        if(noshield){
            *(bool*)((uint64_t) obj + 0x4D) = false;
        }

        if(killall){
            *(bool*)((uint64_t) obj + 0x160) = false;
        }
    }
    oldEnemy(obj);
}

void(*oldHandGrenade)(void* obj);
void HandGrenade(void* obj){
    if(obj != nullptr){
        if(radius){
            *(float*)((uint64_t) obj + 0x24) = 9999999;
        }

        if(damage){
            *(float*)((uint64_t) obj + 0x28) = 9999;
        }

        if(explosiontime){
            *(float*)((uint64_t) obj + 0x20) = 5;
            *(float*)((uint64_t) obj + 0x4C) = 5;
        }
    }
    oldHandGrenade(obj);
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    //Anti-lib rename
    /*
    do {
        sleep(1);
    } while (!isLibraryLoaded("libYOURNAME.so"));*/

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__) //To compile this code for arm64 lib only. Do not worry about greyed out highlighting code, it still works

#else //To compile this code for armv7 lib only.
    Pointers();
    HOOK("0xC0EE34", PlayerHealth, oldPlayerHealth);
    HOOK("0xD0E220", HandGrenade, oldHandGrenade);
    HOOK("0xC226D4", PlayerWeapon, oldPlayerWeapon);
    HOOK("0xED6DE0", Enemy, oldEnemy);
    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}


// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_The Check Box
// To learn HTML, go to this page: https://www.w3schools.com/

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("ButtonLink_Join The Discord_https://discord.gg/fkdDephdS6"), //Not counted
            OBFUSCATE("Category_Account Mods"),
            OBFUSCATE("7_Toggle_Buy All"),
            OBFUSCATE("8_Toggle_Infinite EXP"),
            OBFUSCATE("13_Toggle_Infinite Grenades"),
            OBFUSCATE("14_Toggle_Infinite Medkits"),
            OBFUSCATE("16_Toggle_No Upgrade Duration"),
            OBFUSCATE("Category_Player Mods"),
            OBFUSCATE("1_Toggle_Godmode"),
            OBFUSCATE("Category_Gun Mods"),
            OBFUSCATE("2_Toggle_Infinite Ammo"),
            OBFUSCATE("5_Toggle_Grenade Kill All"),
            OBFUSCATE("6_Toggle_Instant Grenade Explosion"),
            OBFUSCATE("Category_Enemy Mods"),
            OBFUSCATE("4_Toggle_One Shot"),
            OBFUSCATE("11_Toggle_Skip All"),
            OBFUSCATE("10_Toggle_Don't Shoot"),
            OBFUSCATE("12_Toggle_Force No Shield/Helmet"),
    };

    //Now you dont have to manually update the number everytime;
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

void Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    //BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 1:
            health = boolean;
            break;
        case 2:
            ammo = boolean;
            break;
        case 4:
            damage = boolean;
            break;
        case 5:
            radius = boolean;
            break;
        case 6:
            explosiontime = boolean;
            break;
        case 7:
            freeshop = boolean;
            if(freeshop){
                PATCH_SWITCH("0xD68664", "0100A0E31EFF2FE1", freeshop);
                PATCH_SWITCH("0xDC1A20", "0100A0E31EFF2FE1", freeshop);
                PATCH_SWITCH("0xD6A440", "0000A0E31EFF2FE1", freeshop);
                PATCH_SWITCH("0xD68854", "0201E0E31EFF2FE1", freeshop);
                PATCH_SWITCH("0xEBA4E8", "0000A0E31EFF2FE1", freeshop);
            }
            break;
        case 8:
            xp = boolean;
            if(xp){
                PATCH_SWITCH("0xDB3A1C", "0201E0E31EFF2FE1", xp);
            }
            break;
        case 10:
            paused = boolean;
            if(paused){
                PATCH_SWITCH("0xED9DC0", "0100A0E31EFF2FE1", paused);
            }
            break;
        case 12:
            noshield = boolean;
            break;
        case 11:
            killall = boolean;
            break;
        case 13:
            grenades = boolean;
            if(grenades){
                PATCH_SWITCH("0xDB48E8", "0201E0E31EFF2FE1", grenades);
            }
            break;
        case 14:
            medkits = boolean;
            if(medkits){
                PATCH_SWITCH("0xDB4A6C", "0201E0E31EFF2FE1", medkits);
            }
            break;
        case 16:
            developer = boolean;
            if(developer){
                PATCH_SWITCH("0xDB0E2C", "0000A0E31EFF2FE1", developer);
            }
            break;
    }
}

__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

int RegisterMenu(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Icon"), OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(Icon)},
            {OBFUSCATE("IconWebViewData"),  OBFUSCATE("()Ljava/lang/String;"), reinterpret_cast<void *>(IconWebViewData)},
            {OBFUSCATE("IsGameLibLoaded"),  OBFUSCATE("()Z"), reinterpret_cast<void *>(isGameLibLoaded)},
            {OBFUSCATE("Init"),  OBFUSCATE("(Landroid/content/Context;Landroid/widget/TextView;Landroid/widget/TextView;)V"), reinterpret_cast<void *>(Init)},
            {OBFUSCATE("SettingsList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(SettingsList)},
            {OBFUSCATE("GetFeatureList"),  OBFUSCATE("()[Ljava/lang/String;"), reinterpret_cast<void *>(GetFeatureList)},
    };

    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Menu"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterPreferences(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Preferences"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;
    return JNI_OK;
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("CheckOverlayPermission"), OBFUSCATE("(Landroid/content/Context;)V"), reinterpret_cast<void *>(CheckOverlayPermission)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Main"));
    if (!clazz)
        return JNI_ERR;
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) != 0)
        return JNI_ERR;

    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (RegisterMenu(env) != 0)
        return JNI_ERR;
    if (RegisterPreferences(env) != 0)
        return JNI_ERR;
    if (RegisterMain(env) != 0)
        return JNI_ERR;
    return JNI_VERSION_1_6;
}
