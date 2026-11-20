// Ini reader holder: a 4-byte object that owns a 0x40-byte reader block.
//
// Merge evidence for one class: the manifest symbols for sub_0040f9f0,
// sub_0040fa60, sub_0040fb60 and sub_004100e0 all use Object0040f9f0, and
// callers pass the same stack object to the constructor and the release. In
// the EXE function at 0x0044af60, `lea ecx,[ebp-0x84]` feeds both
// sub_0040f9f0 (0x0044b1e8) and sub_0040fa60 (0x0044bb35). Game main
// (sub_00471110) builds, uses and releases one such object on its stack.
#pragma once

#include "msvcp71.h"

struct Object0040f9f0 {
    void* field_00;
    void* sub_0040f9f0();
    void sub_0040fa60();
    void sub_0040fb60(const std::string& fileName, int mode);
    bool sub_004100e0(const std::string& section, const std::string& key, int fallback);
};
