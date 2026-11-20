// GamePlatform0044a3c0 teardown step, called by game main (sub_00471110)
// between shutdown and the game loop. Calls the free routine sub_004d6210
// (writes soundtweak.cfg and releases a global object; no register input), then
// releases the two handles at offsets 0x5c and 0x60 through virtual slot 27
// (vtable offset 0x6c) of the object at 0x00874618 and clears each field.
// The role of the global object and of the two fields is unknown.

// Interface of the global object. Only slot 27 is called here; the preceding
// slots are placeholders that fix its vtable offset.
struct Object00874618 {
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void release(void* handle);
};

extern "C" Object00874618* gObject00874618;
extern "C" void __cdecl sub_004d6210();

struct GamePlatform0044a3c0 {
    unsigned char field_0000[0x5c];
    void* handleA;
    void* handleB;
    void sub_00479e00();
};
typedef char platform_handle_a_assertion[
    ((unsigned int)&((GamePlatform0044a3c0*)0)->handleA) == 0x5c ? 1 : -1];
typedef char platform_handle_b_assertion[
    ((unsigned int)&((GamePlatform0044a3c0*)0)->handleB) == 0x60 ? 1 : -1];

void GamePlatform0044a3c0::sub_00479e00() {
    sub_004d6210();
    if (handleA != 0) {
        gObject00874618->release(handleA);
        handleA = 0;
    }
    if (handleB != 0) {
        gObject00874618->release(handleB);
        handleB = 0;
    }
}
