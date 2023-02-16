//
// Created by letha on 9/4/2021.
//
#include "Mono.h"

#ifndef IMGUIANDROID_FUNCTIONPOINTERS_H
#define IMGUIANDROID_FUNCTIONPOINTERS_H
using namespace BNM::UNITY_STRUCTS;
namespace Pointers{

    void (*addMessage)(void *instance, monoString* chatMessage);
    void (*fullyHeal)(void *instance);
    void LoadPointers()
    {
        addMessage = (void(*)(void*, monoString*)) getAbsoluteAddress("libil2cpp.so", 0x99D4EC);
        fullyHeal = (void(*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x8AB9E4);
    }
#endif IMGUIANDROID_FUNCTIONPOINTERS_H
}