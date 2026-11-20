// GamePlatform0044a3c0: the 0x1bd0-byte platform object that game main
// (sub_00471110) allocates with sub_006efa32(0x1bd0) at 0x004713e0, constructs
// with sub_0044a3c0 and stores in the global at 0x007ca2a0 (0x00471404).
//
// Merge evidence for one class: the manifest symbols for sub_0044a3c0,
// sub_00446bd0, sub_00479e00, sub_0047e860, sub_0047eb00 and sub_004807c0
// already name GamePlatform0044a3c0. The global at 0x007ca2a0 is the `this`
// of every method below. Game main loads it for sub_00446bd0 (0x00471435) and
// sub_00479e00 (0x0047159d). Other EXE code loads the same global for
// sub_0047a0a0 (49 call sites, for example 0x00477e64), sub_0047a1f0 (3) and
// sub_0047a460 (9, for example 0x0052f923). The compiler profiles differ per
// method, so this header builds under both Clang and MSVC 7.1.
//
// Names are hypotheses. sub_0047a0a0 returns the address of the byte at
// +0x70; callers such as 0x00557f3f pass it as `this` to sub_004837b0, so
// +0x70 starts an embedded object of unknown type.
#pragma once

#include "layout.h"
#include "msvcp71.h"

struct GamePlatform0044a3c0 {
    // Slots 0 to 3 are the vtable offsets game main calls: 0x0 (deleting
    // destructor, argument 1), 0x4 (checked), 0x8 and 0xc.
    virtual ~GamePlatform0044a3c0();
    virtual unsigned int platformInit();
    virtual void run();
    virtual void shutdown();

    unsigned char unknown_04[0x58];
    void* handleA;                   // +0x5c, released by sub_00479e00
    void* handleB;                   // +0x60, released by sub_00479e00
    unsigned char unknown_64[0x0c];
    unsigned char field_70;          // +0x70, address returned by sub_0047a0a0
    unsigned char unknown_71[0x51f];
    unsigned int field_590;          // +0x590, written by sub_0047a1f0
    unsigned char unknown_594[0x190];
    unsigned int field_724;          // +0x724, tested by sub_0047a460
    unsigned char unknown_728[0x63c];
    std::string commandLine;         // +0xd64, assigned by sub_00446bd0
    unsigned char unknown_d80[0xe50];

    GamePlatform0044a3c0* sub_0044a3c0(void* instance); // constructor, returns this
    void sub_00446bd0(const std::string& arguments);
    unsigned int sub_0047e860();
    unsigned int sub_0047eb00();
    void sub_00479e00();
    void sub_004807c0();
    unsigned char* sub_0047a0a0();
    void sub_0047a1f0(unsigned int value);
    bool sub_0047a460();
};

LAYOUT_OFFSET(GamePlatform0044a3c0, handleA, 0x5c);
LAYOUT_OFFSET(GamePlatform0044a3c0, handleB, 0x60);
LAYOUT_OFFSET(GamePlatform0044a3c0, field_70, 0x70);
LAYOUT_OFFSET(GamePlatform0044a3c0, field_590, 0x590);
LAYOUT_OFFSET(GamePlatform0044a3c0, field_724, 0x724);
LAYOUT_OFFSET(GamePlatform0044a3c0, commandLine, 0xd64);
LAYOUT_SIZE(GamePlatform0044a3c0, 0x1bd0);
