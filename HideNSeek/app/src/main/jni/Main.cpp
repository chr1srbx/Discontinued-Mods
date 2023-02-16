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


bool coins, points, vip, fireworks, paintings, bedwarsfast, artifcats, atlas, cape, cart, emotion,
bedwarsbig,human, pet, transformer, armor, invis, bunnyhop, fly, untouchable, autorun,kickall, antikick,
build, bypasspass, amogus, fort, murd, chatspam, stealskins, leakname, guns, noclip, god,
doorbypass, damage, buildonce = true;
float cameraheight,camdistance;
int appearance, speed, jump, gravity, ammo, spread;

void(*AddFireworks)(int count);
void(*Build)(void* obj);
bool *(*CloseConnection)(void *player);
void *(*player)();
void (*addChatMessage)(void *player, monoString *message);
bool *(*SetMasterClient)(void *player);
void *(*CleanHUDs)(void *instance);
void (*RefreshPlayerHud)(void *instance, bool isVip, monoString* nickName, monoString* optional);

monoArray<void**> *(*PhotonNetwork_playerListothers)();
Vector3 player_pos;

void Pointers() {
    AddFireworks = (void (*)(int)) getAbsoluteAddress(targetLibName, 0x1112AA4);
    PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) getAbsoluteAddress("libil2cpp.so",0x176169C);
    CloseConnection = (bool *(*)(void *)) getAbsoluteAddress("libil2cpp.so", 0x1766844);//test
    SetMasterClient = (bool *(*)(void *)) getAbsoluteAddress("libil2cpp.so", 0x1766AB8);
    player = (void *(*)()) getAbsoluteAddress("libil2cpp.so", 0x176130C);
    addChatMessage = (void (*)(void *, monoString *)) getAbsoluteAddress("libil2cpp.so", 0x10CAD0C);
    RefreshPlayerHud = (void (*)(void *, bool, monoString*, monoString*)) getAbsoluteAddress("libil2cpp.so",0x1058E10);
    Build = (void(*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x10C8620);
}

void (*old_CharacterMotor)(void *instance);
void CharacterMotor(void *instance){
    if (instance != nullptr){
        if(speed != NULL){
            *(float*) ((uint64_t) instance + 0x24) = speed;
        }
        if(jump != NULL){
            *(float*) ((uint64_t) instance + 0x2C) = jump;
        }

        if(gravity != NULL){
            *(float*) ((uint64_t) instance + 0x34) = gravity;
        }

        if(bunnyhop){
            *(bool*) ((uint64_t) instance + 0x28) = true;
            *(bool*) ((uint64_t) instance + 0x30) = true;
        }
    }
    old_CharacterMotor(instance);
}

void (*oldDoBeforeShown)(void *instance);
void DoBeforeShown(void *instance)
{
    if (instance != nullptr && bypasspass)
    {
        *(monoString**)((uint64_t)instance+0x48) = CreateMonoString("");
    }
    oldDoBeforeShown(instance);
}

void (*old_PlayerUpdate)(void *instance);
void PlayerUpdate(void *instance) {
    if (instance != nullptr) {
        auto photonplayers = PhotonNetwork_playerListothers();
        if (kickall) {
            SetMasterClient(player());
            for (int i = 0; i < photonplayers->getLength(); ++i) {
                auto photonplayer = photonplayers->getPointer()[i];
                CloseConnection(photonplayer);
            }
        }

        if (god) {
            *(float *) ((uint64_t) instance + 0x178) = 999;
        }

        if (fly) {
            *(bool *) ((uint64_t) instance + 0x50) = true;
            *(bool *) ((uint64_t) instance + 0xE7) = true;
        }

       /* if (untouchable) {
            *(bool *) ((uint64_t) instance + 0x48) = true;
            *(float *) ((uint64_t) instance + 0x184) = 9999;
        }*/

        if (fireworks) {
            AddFireworks(99);
        }

        void* NetworkMsgPlayerAppearanceData = *(void**)((uint64_t) instance + 0x7C);
        if(NetworkMsgPlayerAppearanceData != nullptr){
            if(invis){
                *(int*)((uint64_t) NetworkMsgPlayerAppearanceData + 0x8) = 6;
            }
        }
    }
    old_PlayerUpdate(instance);
}

void(*oldGunFpvController)(void* instance);
void GunFpvController(void *instance){
    if(instance != nullptr){
        if(spread != NULL){
            *(float *) ((uint64_t) instance + 0x28) = spread;
            *(float *) ((uint64_t) instance + 0x2C) = spread;
        }

        if(ammo != NULL) {
            *(int *) ((uint64_t) instance + 0x34) = ammo;
        }

        void* commonSettings = *(void**)((uint64_t) instance + 0x58);
        if(commonSettings != nullptr){
            if(damage){
                *(float *) ((uint64_t) commonSettings + 0xC) = 999;
            }
        }
    }
    oldGunFpvController(instance);
}

void (*oldPlayerLookAround)(void *instance);
void PlayerLookAround(void *instance)
{
    if (instance != nullptr)
    {
        void* Localplayer = *(void* *)((uint64_t)instance + 0x24);
        if(Localplayer != nullptr){
            if(chatspam) {
                addChatMessage(Localplayer, CreateMonoString("chr1s on top, get the hax at https://discord.gg/fkdDephdS6"));
            }
        }
    }
    oldPlayerLookAround(instance);
}

void (*oldRemotePlayer)(void *instance);
void RemotePlayer(void *instance)
{
    if (instance != nullptr)
    {
        if(stealskins){
            *(bool*)((uint64_t) instance + 0x101) = true;
        }
    }
    oldRemotePlayer(instance);
}

int(*oldgetRole)(void *instance);
int getRole(void *instance){
    if(instance != nullptr && murd){
        return 1;
    }
    return oldgetRole(instance);
}

void (*oldPanelPopupDoorGuard)(void* instance);
void PanelPopupDoorGuard(void* instance){
    if(instance != nullptr){
        void* GuardData = *(void**)((uint64_t) instance + 0x48);
        if(GuardData != nullptr && doorbypass){
            *(int*)((uint64_t) GuardData + 0x20) = 0000;
        }
    }
    oldPanelPopupDoorGuard(instance);
}

void (*oldBedWarsSummonerDrop)(void* obj);
void BedWarsSummonerDrop(void* obj){
    if(obj != nullptr){
        if(bedwarsfast){
            *(float*)((uint64_t) obj + 0x28) = 0;
        }

        if(bedwarsbig){
            *(int*)((uint64_t) obj + 0x30) = 999;
        }
    }
    oldBedWarsSummonerDrop(obj);
}

void(*oldOnLogin)(void* obj);
void OnLogin(void* obj){
    if(obj != nullptr){
        *(_monoString**)((uint64_t) obj + 0xC) = CreateMonoString("moddedbychr1s");
    }
    oldOnLogin(obj);
}

// we will run our hacks in a new thread so our while loop doesn't block process main thread
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
    PATCH("0x10C0A58", "00F020E31EFF2FE1");
    PATCH("0x11243F0", "00F020E31EFF2FE1");
    PATCH("0x1793010", "1A00A0E3EFF2FE1");
    PATCH("0x17947C8", "1A00A0E3EFF2FE1");
    HOOK("0x1906DBC", CharacterMotor, old_CharacterMotor);
    HOOK("0x114C010", PlayerUpdate, old_PlayerUpdate);
    HOOK("0x13D00C8", DoBeforeShown, oldDoBeforeShown);
    HOOK("0x1165B68", GunFpvController, oldGunFpvController);
    HOOK("0x134A43C", getRole, oldgetRole);
    HOOK("0x114F0F8", PlayerLookAround, oldPlayerLookAround);
    HOOK("0x14AE0D4", RemotePlayer, oldRemotePlayer);
    HOOK("0x13C2B14", PanelPopupDoorGuard, oldPanelPopupDoorGuard);
    HOOK("0x11B3460", BedWarsSummonerDrop, oldBedWarsSummonerDrop);
    HOOK("0x10EC990", OnLogin, oldOnLogin);
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
            OBFUSCATE("Category_Account Mods"), //Not counted
            OBFUSCATE("Toggle_Coins"),//0
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Coins.</font>"),
            OBFUSCATE("Toggle_Points"),//1
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Points.</font>"),
            OBFUSCATE("Toggle_Paintings"),//2
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Paintings.</font>"),
            OBFUSCATE("Toggle_Fireworks"),//3
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Fireworks.</font>"),
            OBFUSCATE("Toggle_VIP"),//4
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks VIP for free.</font>"),
            OBFUSCATE("Toggle_Unlock Albums"),//5
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Album.</font>"),
            OBFUSCATE("Toggle_Unlock Artifacts"),//6
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Artifact.</font>"),
            OBFUSCATE("Toggle_Unlock Armors"),//7
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Armor.</font>"),
            OBFUSCATE("Toggle_Unlock Atlases/Textures"),//8
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Atlas.</font>"),
            OBFUSCATE("Toggle_Unlock Capes"),//9
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Cape.</font>"),
            OBFUSCATE("Toggle_Unlock Carts"),//10
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Cart.</font>"),
            OBFUSCATE("Toggle_Unlock Emotions/Animation"),//11
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Emotion/Animation.</font>"),
            OBFUSCATE("Toggle_Unlock Guns"),//12
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Gun</font>"),
            OBFUSCATE("Toggle_Unlock Humans"),//13
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Human</font>"),
            OBFUSCATE("Toggle_Unlock Pets"),//14
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Pet and the paywall to actually equip one.</font>"),
            OBFUSCATE("Toggle_Unlock Transformer"),//15
            OBFUSCATE("RichTextView_<font color='lightgrey'>Unlocks Every Transformer.</font>"),
            OBFUSCATE("Category_Player Mods"),
            OBFUSCATE("37_Toggle_Noclip"),//37
            OBFUSCATE("RichTextView_<font color='lightgrey'>Walk through walls. (Use Fly or you'll fall out the map)</font>"),
            OBFUSCATE("60_Spinner_Player Type_Invalid,Modern,Player,Human,Others,MovingBlock,StaticBlock"),//37
            OBFUSCATE("RichTextView_<font color='lightgrey'>Chages the player type (to a block etc).</font>"),
            OBFUSCATE("Toggle_Godmode"),//16
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Gives you infinite health</font>"),
            OBFUSCATE("SeekBar_Speed_0_200"),//17
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Speed the player will have.</font>"),
            OBFUSCATE("SeekBar_Jump Height_0_200"),//18
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Jump Height the player will have.</font>"),
            OBFUSCATE("SeekBar_Gravity_0_200"),//19
            OBFUSCATE("RichTextView_<font color='lightgrey'><font color='lightgrey'>Sets the amount of Gravity the player will have.</font>"),
            OBFUSCATE("Toggle_Invisible"),//20
            OBFUSCATE("RichTextView_<font color='lightgrey'>Make yourself completely invisible</font>"),
            OBFUSCATE("Toggle_BunnyHop"),//21
            OBFUSCATE("Toggle_Fly"),//22
            OBFUSCATE("RichTextView_<font color='lightgrey'>Get the ability to Fly.</font>"),
            OBFUSCATE("Category_Server Mods"),
            OBFUSCATE("25_Toggle_Kick All"),//25
            OBFUSCATE("RichTextView_<font color='lightgrey'>Will disconnect everyone from the server.</font>"),
            OBFUSCATE("26_Toggle_Anti-Kick"),//26
            OBFUSCATE("RichTextView_<font color='lightgrey'>Ignores any kick requests from the game's owner.</font>"),
            OBFUSCATE("27_Toggle_Bypass Password"),//27
            OBFUSCATE("RichTextView_<font color='lightgrey'>Bypasses password locked servers (just press enter, don't type anything)</font>"),
            OBFUSCATE("33_Toggle_Chat Spam"),//33
            OBFUSCATE("RichTextView_<font color='lightgrey'>Spams the chat.</font>"),
            OBFUSCATE("34_Toggle_Steal Skins"),//34
            OBFUSCATE("RichTextView_<font color='lightgrey'>Allows the user to get any skin from any user.</font>"),
            OBFUSCATE("38_Toggle_Bypass Door Password"),//38
            OBFUSCATE("RichTextView_<font color='lightgrey'>Bypasses locked doors. (just press enter, don't type anything)</font>"),
            OBFUSCATE("Category_CopsNCrims Mods"),
            OBFUSCATE("28_SeekBar_Gun Spread_0_100"),//28
            OBFUSCATE("RichTextView_<font color='lightgrey'>Sets the amount of spread the weapon will have</font>"),
            OBFUSCATE("29_SeekBar_Ammo_0_100"),//29
            OBFUSCATE("RichTextView_<font color='lightgrey'>Sets the amount of ammo in the magazine.</font>"),
            OBFUSCATE("41_Toggle_One Shot Kill"),//41
            OBFUSCATE("RichTextView_<font color='lightgrey'>Every weapon will 1 shot kill anyone.</font>"),
            OBFUSCATE("Category_Murder Mystery Mods"),
            OBFUSCATE("30_Toggle_Become Murder"),//30
            OBFUSCATE("Category_Among Impostors Mods"),
            OBFUSCATE("31_Toggle_Become Impostor"),//31
            OBFUSCATE("Category_Battle Royale Mods"),
            OBFUSCATE("32_Toggle_Don't take damage in Danger Area"),//32
            OBFUSCATE("Category_Bedwars Mods"),
            OBFUSCATE("42_Toggle_Big Spawns"),//42
            OBFUSCATE("RichTextView_<font color='lightgrey'>Spawns a large amount of resources at the summoner.</font>"),
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
        case 0:
            coins = boolean;
            PATCH_SWITCH("0x1426F4C", "12 07 80 E3 1E FF 2F E1", coins);
            break;
        case 1:
            points = boolean;
            PATCH_SWITCH("0x1426E40", "12 07 80 E3 1E FF 2F E1", points);
            break;
        case 2:
            paintings = boolean;
            PATCH_SWITCH("0x1427058", "12 07 80 E3 1E FF 2F E1", paintings);
            break;
        case 3:
            fireworks = boolean;
            break;
        case 4:
            vip = boolean;
            PATCH_SWITCH("0x1427164", "01 00 A0 E3 1E FF 2F E1", vip);
            break;
        case 5:
            armor = boolean;
            PATCH_SWITCH("0x110FB50", "01 00 A0 E3 1E FF 2F E1", armor);
            break;
        case 6:
            artifcats = boolean;
            PATCH_SWITCH("0x110FB50", "01 00 A0 E3 1E FF 2F E1", artifcats);
            break;
        case 7:
            armor = boolean;
            PATCH_SWITCH("0x110FEC4", "01 00 A0 E3 1E FF 2F E1",armor);
            break;
        case 8:
            atlas = boolean;
            PATCH_SWITCH("0x110F788", "01 00 A0 E3 1E FF 2F E1", atlas);
            break;
        case 9:
            cape = boolean;
            PATCH_SWITCH("0x1110238", "01 00 A0 E3 1E FF 2F E1", cape);
        case 10:
            cart = boolean;
            PATCH_SWITCH("0x1111268", "01 00 A0 E3 1E FF 2F E1", cart);
            break;
        case 11:
            emotion = boolean;
            PATCH_SWITCH("0x1110EF4", "01 00 A0 E3 1E FF 2F E1", emotion);
            break;
        case 12:
            guns = boolean;
            PATCH_SWITCH("0x1110EF4", "01 00 A0 E3 1E FF 2F E1", guns);
            break;
        case 13:
            human = boolean;
            PATCH_SWITCH("0x1112EA8", "01 00 A0 E3 1E FF 2F E1", human);
            break;
        case 14:
            pet = boolean;
            PATCH_SWITCH("0x11139DC", "01 00 A0 E3 1E FF 2F E1", pet);
            PATCH_SWITCH("0x142727C", "01 00 A0 E3 1E FF 2F E1", pet);
            break;
        case 15:
            transformer = boolean;
            PATCH_SWITCH("0x1111A1C", "01 00 A0 E3 1E FF 2F E1", transformer);
            break;
        case 16:
            god = value;
            break;
        case 17:
            speed = value;
            break;
        case 18:
            jump = value;
            break;
        case 19:
            gravity = value;
            break;
        case 20:
            invis = boolean;
            break;
        case 21:
            bunnyhop = boolean;
            break;
        case 22:
            fly = boolean;
            break;
        case 23:
            autorun = boolean;
            break;
        case 24:
            untouchable = boolean;
            break;
        case 25:
            kickall = boolean;
            break;
        case 26:
            antikick = boolean;
            PATCH_SWITCH("0x1161544", "00 00 A0 E3 1E FF 2F E1", antikick);
            break;
        case 27:
            bypasspass = boolean;
            break;
        case 28:
            spread = value;
            break;
        case 29:
            ammo = value;
            break;
        case 30:
            murd = boolean;
            break;
        case 31:
            amogus = boolean;
            PATCH_SWITCH("0x10938C0", "01 00 A0 E3 1E FF 2F E1", amogus);
            break;
        case 32:
            fort = boolean;
            PATCH_SWITCH("0x1155964", "00 00 A0 E3 1E FF 2F E1", fort);
            break;
        case 33:
            chatspam = boolean;
            break;
        case 34:
            stealskins = boolean;
            break;
        case 36:
            guns = boolean;
            break;
        case 37:
            noclip = boolean;
            PATCH_SWITCH("0x18AC0F8", "00 00 A0 E3 1E FF 2F E1", noclip);
            PATCH_SWITCH("0x18B8DB0", "00 00 A0 E3 1E FF 2F E1", noclip);
            break;
        case 38:
            doorbypass = boolean;
            break;
        case 41:
            damage = boolean;
            break;
        case 42:
            bedwarsbig = boolean;
            break;
        case 43:
            bedwarsfast = boolean;
            break;
        case 50:
            build = boolean;
            break;
        case 60:
            appearance = value;
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
