struct Il2CppObject
{
    void* klass;
    void* monitor;
};
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

typedef struct _monoString {
    void *klass;
    void *monitor;
    int length;
    char chars[1];

    int getLength() {
        return length;
    }

    char *getChars() {
        return chars;
    }
} monoString;

monoString *CreateMonoString(const char *str) {
    monoString *(*String_CreateString)(void *_this, const char *str) = (monoString *(*)(void *, const char *))getAbsoluteAddress("libil2cpp.so", 0x2054574);
    return String_CreateString(NULL, str);
}