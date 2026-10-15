#include <cassert>
#include <initializer_list>
#include <cmath>
#include <limits>
#include "../src/reconstruction/sub_00401000.cpp"
#include "../src/reconstruction/sub_004068e0.cpp"
#include "../src/reconstruction/sub_00406490.cpp"
int main() {
    Object00401000 a = {};
    Object004068e0 b = {};
    for (unsigned int value : {0u, 1u, 0x80000000u, 0xffffffffu}) {
        a.field_0c = value;
        assert(a.sub_00401000() == value);
        b.sub_004068e0(value);
        assert(b.field_3c == value);
        for (unsigned char byte : b.unknown_00) assert(byte == 0);
    }
    assert(sub_00406490(-3.5f) == 3.5f);
    assert(!std::signbit(sub_00406490(-0.0f)));
    assert(std::isinf(sub_00406490(-std::numeric_limits<float>::infinity())));
    assert(std::isnan(sub_00406490(std::numeric_limits<float>::quiet_NaN())));
}
