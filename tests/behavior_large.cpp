#include <assert.h>
#include <limits>

#include "../src/reconstruction/sub_006de8a0.cpp"

static void run(int count, const float* mapped, const float* delta,
                const float* source, float* output, float maximum = 10.0f) {
    State006de8a0 state = {};
    state.bias[1] = 2.0f;
    state.maximum = maximum;
    const float* channels[] = {0, mapped};
    Context006de8a0 context = {count, &state, 0, channels};
    sub_006de8a0(&context, delta, source, 1, output);
}

int main() {
    const float mapped[] = {1, 15, -5, 4, 3, 20, -10, 2};
    const float delta[] = {1, 0, 0, 0, 0, -15, 5, 1};
    const float source[] = {0, 0, 5, 0, 0, 3, 0, 0};
    float output[] = {-99, -99, -99, -99, -99, -99, -99, -99};

    run(0, mapped, delta, source, output);
    for (int i = 0; i < 8; ++i) assert(output[i] == -99);
    run(-1, mapped, delta, source, output);
    run((-2147483647 - 1), mapped, delta, source, output);
    for (int i = 0; i < 8; ++i) assert(output[i] == -99);

    run(4, mapped, delta, source, output);
    assert(output[0] == 2);  // lower clamp
    assert(output[1] == 10); // upper clamp
    assert(output[2] == 7);  // lower clamp after a low mapped value
    assert(output[3] == 4);

    run(6, mapped, delta, source, output);
    assert(output[4] == 3);  // four-item loop plus two-item tail
    assert(output[5] == 5);

    run(8, mapped, delta, source, output);
    assert(output[6] == 2 && output[7] == 3); // second four-item iteration

    float aliased[] = {0, 0, 5, 0};
    run(4, mapped, delta, aliased, aliased);
    assert(aliased[0] == 2 && aliased[1] == 10);
    assert(aliased[2] == 7 && aliased[3] == 4);

    float shifted[] = {0, 0, 5, 0, 0};
    run(4, mapped, delta, shifted, shifted + 1);
    assert(shifted[1] == 2 && shifted[2] == 10);
    assert(shifted[3] == 12 && shifted[4] == 14);

    const float nan = std::numeric_limits<float>::quiet_NaN();
    const float nanMapped[] = {nan};
    const float one[] = {1};
    const float zero[] = {0};
    float nanOutput[] = {0};
    run(1, nanMapped, one, one, nanOutput);
    assert(nanOutput[0] == 3); // unordered upper result selects the lower bound

    const float ordinary[] = {5};
    const float nanSource[] = {nan};
    run(1, ordinary, zero, nanSource, nanOutput);
    assert(nanOutput[0] != nanOutput[0]); // unordered lower bound is selected
    run(1, ordinary, zero, one, nanOutput, nan);
    assert(nanOutput[0] == 5); // unordered upper limit leaves the value alone
}
