
int GetObscuredIntValue(uint64_t location){
    int cryptoKey = *(int *)location;
    int obfuscatedValue = *(int *)(location + 0x4);

    return obfuscatedValue ^ cryptoKey;
}

/*
Set the real value of an ObscuredInt.
Parameters:
	- location: the location of the ObscuredInt
	- value: the value we're setting the ObscuredInt to
*/
void SetObscuredIntValue(uint64_t location, int value){
    int cryptoKey = *(int *)location;

    *(int *)(location + 0x4) = value ^ cryptoKey;
}

/*
Get the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
*/
float GetObscuredFloatValue(uint64_t location){
    int cryptoKey = *(int *)location;
    int obfuscatedValue = *(int *)(location + 0x4);

    union intfloat {
        int i;
        float f;
    };

    /* use this intfloat to set the integer representation of our parameter value, which will also set the float value */
    intfloat IF;
    IF.i = obfuscatedValue ^ cryptoKey;

    return IF.f;
}

/*
Set the real value of an ObscuredFloat.
Parameters:
	- location: the location of the ObscuredFloat
	- value: the value we're setting the ObscuredFloat to
*/
void SetObscuredFloatValue(uint64_t location, float value){
    int cryptoKey = *(int *)location;

    union intfloat {
        int i;
        float f;
    };

    /* use this intfloat to get the integer representation of our parameter value */
    intfloat IF;
    IF.f = value;

    /* use this intfloat to generate our hacked ObscuredFloat */
    intfloat IF2;
    IF2.i = IF.i ^ cryptoKey;

    *(float *)(location + 0x4) = IF2.f;
}