#ifndef MENU:
#define MENU
#include "Misc/Logging.h"
#include "Misc/ESP.h"
#include "Misc/Unity.h"
namespace Menu
{

    bool spawncash,freeshop,ebrake, tirespop ,invicible, invisible, grenades, nospread,recoil,spamchat, roundtime, money, kickall;
    float health, speed, radius,rawrpm,rpm,accelerationspeed,deaccelerationspeed,cardamage, rawRPM, rpmbiascurvelimitfreeshop;
    int damageforce, damage, range, ammo;
    int (*Screen_get_height)();

    void (*oldMPPlayerController)(void* obj);
    void MPPlayerController(void* obj){
        if(obj != nullptr) {
            if (health != NULL) {
                *(float *) ((uint64_t) obj + 0x18) = health;
            }

            if (invicible) {
                *(bool *) ((uint64_t) obj + 0x15) = true;
            }

            if (invisible) {
                *(bool *) ((uint64_t) obj + 0xB0) = true;
            }

            if(speed != NULL){
                *(float *) ((uint64_t) obj + 0x100) = speed;
                *(float *) ((uint64_t) obj + 0xF0) = speed;
                *(float *) ((uint64_t) obj + 0x20) = speed;
                *(float *) ((uint64_t) obj + 0x1C) = speed;
            }

        }
        oldMPPlayerController(obj);
    }

    void (*oldPlayerController)(void* obj);
    void PlayerController(void* obj){
        if(obj != nullptr) {
            if (health != NULL) {
                *(float *) ((uint64_t) obj + 0x18) = health;
            }

            if(speed != NULL){
                *(float *) ((uint64_t) obj + 0x70) = speed;
                *(float *) ((uint64_t) obj + 0x6C) = speed;
            }
            if (invicible) {
                *(bool *) ((uint64_t) obj + 0x15) = true;
            }

            if (invisible) {
                *(bool *) ((uint64_t) obj + 0xB0) = true;
            }

        }
        oldPlayerController(obj);
    }

    void (*oldMPWeapon)(void* obj);
    void MPWeapon(void* obj){
        if(obj != nullptr) {
            void* CurrentGun = *(void**)((uint64_t) obj + 0x2C);
            if(nospread){
                *(float *) ((uint64_t) CurrentGun + 0x58) = 0;
                *(float *) ((uint64_t) CurrentGun + 0x50) = 0;
                *(float *) ((uint64_t) CurrentGun + 0x4C) = 0;
            }

            if(freeshop){
                *(float *) ((uint64_t) CurrentGun + 0x5C) = 999999999;
            }
            if(damageforce != NULL){
                *(int *) ((uint64_t) CurrentGun + 0x48) = damageforce;
            }

            if(damage != NULL){
                *(float *) ((uint64_t) CurrentGun + 0x60) = damage;
                *(float *) ((uint64_t) CurrentGun + 0x64) = damage;
            }

            if(range != NULL){
                *(float *) ((uint64_t) CurrentGun + 0x6C) = range;
            }

            if(ammo != NULL){
                *(float *) ((uint64_t) CurrentGun + 0x88) = ammo;
                *(float *) ((uint64_t) CurrentGun + 0x80) = ammo;
            }

            if(recoil){
                *(float *) ((uint64_t) CurrentGun + 0x48) = 0;
            }
        }
        oldMPWeapon(obj);
    }

    void (*oldWeapon)(void* obj);
    void Weapon(void* obj){
        if(obj != nullptr) {
            void* Shooter = *(void**)((uint64_t) obj + 0x30);
            if(nospread){
                *(float *) ((uint64_t) Shooter + 0x3C) = 0;
            }

            if(damageforce != NULL){
                *(int *) ((uint64_t) Shooter + 0x48) = damageforce;
            }

            if(damage != NULL){
                *(float *) ((uint64_t) Shooter + 0x44) = damage;
            }

            if(range != NULL){
                *(float *) ((uint64_t) Shooter + 0x4C) = range;
            }

            if(ammo != NULL){
                *(float *) ((uint64_t) Shooter + 0x5C) = ammo;
                *(float *) ((uint64_t) Shooter + 0x58) = ammo;
            }

            if(recoil){
                *(float *) ((uint64_t) Shooter + 0x38) = 0;
            }
        }
        oldWeapon(obj);
    }

    void(*oldWheel)(void* obj);
    void Wheel(void* obj){
        if(obj != nullptr){
            if(ebrake){
                *(bool *) ((uint64_t) obj + 0x140) = true;
            }

            if(tirespop){
                *(bool *) ((uint64_t) obj + 0xDB) = false;
            }
        }
        oldWheel(obj);
    }

    void(*oldGameManager)(void* obj);
    void GameManager(void* obj){
        if(obj != nullptr){
            if(grenades){
                *(int *) ((uint64_t) obj + 0x40) = 99999;
            }
        }
        oldGameManager(obj);
    }

    void(*oldMPGameManager)(void* obj);
    void MPGameManager(void* obj){
        if(obj != nullptr){
            if(grenades){
                *(int *) ((uint64_t) obj + 0x10C) = 99999;
            }
        }
        oldMPGameManager(obj);
    }

    int (*Screen_get_width)();
    ImVec4 color = ImVec4(1, 1, 1, 1);

    void DrawMenu() {
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {
            ImGui::StyleColorsDark();
            ImGui::Begin("Dude Theft Wars - chr1s - https//discord.gg/fkdDephdS6");
            if (ImGui::CollapsingHeader("Player Mods")) {
                ImGui::Checkbox("Invisisble", &invisible);
                ImGui::SliderFloat("Health", &health, 0.0, 1000.0);
                ImGui::SliderFloat("Speed", &speed, 0.0, 500.0);
            }
            if (ImGui::CollapsingHeader("Weapon Mods")) {
                ImGui::Checkbox("No Spread", &nospread);
                ImGui::Checkbox("Grenade Kill All", &grenades);
                ImGui::SameLine();
                ImGui::Checkbox("No Recoil", &recoil);
                ImGui::SliderInt("Damage Force", &damageforce, 0, 1000);
                ImGui::SliderInt("Damage", &damage, 0, 1000);
                ImGui::SliderInt("Bullet Range", &range, 0, 1000);
                ImGui::SliderInt("Ammo", &ammo, 0, 1000);
            }
            if (ImGui::CollapsingHeader("Car Mods")) {
                ImGui::Checkbox("Ebrake", &ebrake);
                ImGui::SameLine();
                ImGui::Checkbox("Unpoppable Tires", &tirespop);
            }
            ImGui::End();
        }
    }
    void DrawImGui()
    {
        if (init && Screen_get_height)
        {
            ImGuiIO &io = ImGui::GetIO();
            static bool WantTextInputLast = false;
//            if (io.WantTextInput && !WantTextInputLast)
//                displayKeyboard(true);
//            WantTextInputLast = io.WantTextInput;
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplAndroid_NewFrame(Screen_get_width(), Screen_get_height());
            ImGui::NewFrame();
            DrawMenu();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_PageUp]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_PageDown]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Home]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_End]] = false;
            io.KeysDown[io.KeyMap[ImGuiKey_Insert]] = false;

            ImGui::EndFrame();
        }
    }
}
#endif MENU