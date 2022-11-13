#pragma once
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>
#include <dlfcn.h>

#define targetLibName OBFUSCATE("libil2cpp.so")


typedef struct _monoString {
    void *klass;
    void *monitor;
    int length;
    char chars[255];

    const char & operator[](size_t i) const
    {
        return (chars[i]);
    }

    char & operator[](size_t i)
    {
        return (chars[i]);
    }

    std::string get_string() {
        std::u16string u16_string(reinterpret_cast<const char16_t *>(chars));
        std::wstring wide_string(u16_string.begin(), u16_string.end());
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        return convert.to_bytes(wide_string);
    }

    void create(const char *s) {
        std::string str = std::string(s);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        std::wstring wa = convert.from_bytes(str);
        length = str.length();
        std::u16string basicString = std::u16string(wa.begin(), wa.end());
        const char16_t *cStr = basicString.c_str();
        memcpy(getChars(), cStr, getLength() * 2);
    }

    void create(std::string str) {
        create(str.c_str());
    }

    static _monoString *New(const char *str) {
        _monoString *s = (_monoString *) calloc(1, (sizeof(_monoString) + ((strlen(str) + 1) * 2)));
        s->create(str);
        return s;
    }

    static _monoString *New(std::string str) {
        return New(str.c_str());
    }

    const char *get_const_char() {
        return get_string().c_str();
    }

    char *getChars() {
        return chars;
    }

    int getLength() {
        return length;
    }

} monoString;
monoString *CreateMonoString(const char *str) {
        monoString *(*String_CreateString)(void *instance, const char *str);
        String_CreateString = (monoString *(*)(void *,const char *)) getAbsoluteAddress(targetLibName, 0xA1AF30);
        return String_CreateString(NULL, str);
    }

monoString *CreateMonoString(std::string str, bool il2cpp = true) {
    return CreateMonoString(str.c_str(), il2cpp);
}

template <typename T>
struct monoArray
{
    void* klass;
    void* monitor;
    void* bounds;
    int   max_length;
    void* vector [1];
    int getLength()
    {
        return max_length;
    }
    T getPointer()
    {
        return (T)vector;
    }
};

template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};
