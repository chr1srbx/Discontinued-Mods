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
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu/Setup.h"
#include "Includes/Macros.h"
#include "Unity/Unity.h";

#define targetLibName OBFUSCATE("libil2cpp.so")


bool buyall, damage, kickall, fuel, coins, freecar;
float horse, torque;
int tyretype;
void *instanceBtn;
void(*UpdateMoney)(float moneyInput);
bool *(*SetMasterClient)(void *player);
bool *(*CloseConnection)(void *player);
monoArray<void**> *(*PhotonNetwork_playerListothers)();
void *(*player)();
void Pointers(){
    UpdateMoney = (void(*)(float) ) getAbsoluteAddress(targetLibName, 0x2F3E490);
    PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) getAbsoluteAddress("libil2cpp.so",0x270F308);
    CloseConnection = (bool *(*)(void *)) getAbsoluteAddress("libil2cpp.so", 0x27133E8);//test
    SetMasterClient = (bool *(*)(void *)) getAbsoluteAddress("libil2cpp.so", 0x2713610);
    player = (void *(*)()) getAbsoluteAddress("libil2cpp.so", 0x270F010);
}

float(*oldCarPrice)(void* obj);
float CarPrice(void* obj){
    if(obj != nullptr && freecar){
        return 0;
    }
    oldCarPrice(obj);
}

void(*oldCarDamage)(void* obj);
void CarDamage(void* obj){
    if(obj != nullptr && damage){
        *(bool*)((uint64_t) obj + 0xD8) = false;
        *(bool*)((uint64_t) obj + 0xD8) = false;
        *(float*)((uint64_t) obj + 0x30) = 0;
        *(float*)((uint64_t) obj + 0x34) = 0;
        *(float*)((uint64_t) obj + 0x58) = 0;
        *(float*)((uint64_t) obj + 0x5C) = 0;
    }
    oldCarDamage(obj);
}

void(*oldDrivetrain)(void* obj);
void Drivetrain(void* obj){
    if(obj != nullptr){
        auto photonplayers = PhotonNetwork_playerListothers();
        if (kickall) {
            SetMasterClient(player());
            for (int i = 0; i < photonplayers->getLength(); ++i) {
                auto photonplayer = photonplayers->getPointer()[i];
                CloseConnection(photonplayer);
            }
        }

        if(horse != NULL){
            *(float*)((uint64_t) obj + 0x78) = horse;
            *(float*)((uint64_t) obj + 0x1C8) = horse;
            *(float*)((uint64_t) obj + 0x8C) = horse;
            *(float*)((uint64_t) obj + 0x1C4) = horse;
        }

        if(torque){
            *(float*)((uint64_t) obj + 0x80) = torque;
        }

        void* FuelTank = *(void**)((uint64_t) obj + 0x50);
        if(FuelTank != nullptr){
            if(fuel){
                *(float*)((uint64_t) FuelTank + 0x48) = MAXFLOAT;
                *(float*)((uint64_t) FuelTank + 0x50) = MAXFLOAT;
                *(float*)((uint64_t) FuelTank + 0x44) = MAXFLOAT;
            }
        }
    }
    oldDrivetrain(obj);
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
    Pointers();
    PATCH("0x2F4B024", "1F2003D5C0035FD6");
    HOOK("0x2F694D8", Drivetrain, oldDrivetrain);
    HOOK("0x2F30FE0", CarPrice, oldCarPrice);
    HOOK("0x29A0460", CarDamage, oldCarDamage);
#else //To compile this code for armv7 lib only.
    LOGI(OBFUSCATE("Done"));
#endif

    //Anti-leech
    /*if (!iconValid || !initValid || !settingsValid) {
        //Bad function to make it crash
        sleep(5);
        int *p = 0;
        *p = 0;
    }*/

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
            OBFUSCATE("Category_Account Mods"), //Not counted
            OBFUSCATE("1_Toggle_Free Car Upgrades"),
            OBFUSCATE("7_Toggle_Free Cars"),
            OBFUSCATE("Category_Car Mods"), //Not counted
            OBFUSCATE("3_SeekBar_Car HorsePower_0_4000"),
            OBFUSCATE("4_SeekBar_Car Torque_0_4000"),
            OBFUSCATE("2_Toggle_No Damage"),
            OBFUSCATE("5_Toggle_Infinite Fuel"),
            OBFUSCATE("Category_Server Mods"), //Not counted
            OBFUSCATE("6_Toggle_Kick All"),
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
            buyall = boolean;
            PATCH_SWITCH("0x2F3E974", "200080D2C0035FD6", buyall);
            break;
        case 2:
            damage = boolean;
            break;
        case 3:
            horse = value;
            break;
        case 4:
            torque = value;
            break;
        case 5:
            fuel = boolean;
            break;
        case 6:
            kickall = boolean;
            break;
        case 7:
            freecar = boolean;
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
