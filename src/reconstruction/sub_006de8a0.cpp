#include "layout.h"
struct State006de8a0 {
    float unknown_00[3];
    float bias[24];
    float maximum;
};

struct Context006de8a0 {
    int count;
    State006de8a0* state;
    void* unknown_08;
    const float** mapped;
};

LAYOUT_OFFSET(State006de8a0, bias, 0x0c);
LAYOUT_OFFSET(State006de8a0, maximum, 0x6c);
LAYOUT_OFFSET(Context006de8a0, state, 4);
LAYOUT_OFFSET(Context006de8a0, mapped, 0x0c);

// FUNCTION: RBR 0x006de8a0
extern "C" void sub_006de8a0(Context006de8a0* context, const float* delta,
                              const float* source, int channel, float* output) {
    const int count = context->count;
    const float bias = context->state->bias[channel];
    int i = 0;

    if (count >= 4) {
        do {
            float value = context->mapped[channel][i] + delta[i];
            if (value > context->state->maximum) value = context->state->maximum;
            if (!(value > bias + source[i])) value = bias + source[i];
            output[i] = value;

            value = context->mapped[channel][i + 1] + delta[i + 1];
            if (value > context->state->maximum) value = context->state->maximum;
            if (!(value > bias + source[i + 1])) value = bias + source[i + 1];
            output[i + 1] = value;

            value = context->mapped[channel][i + 2] + delta[i + 2];
            if (value > context->state->maximum) value = context->state->maximum;
            if (!(value > bias + source[i + 2])) value = bias + source[i + 2];
            output[i + 2] = value;

            value = context->mapped[channel][i + 3] + delta[i + 3];
            if (value > context->state->maximum) value = context->state->maximum;
            if (!(value > bias + source[i + 3])) value = bias + source[i + 3];
            output[i + 3] = value;
            i += 4;
        } while (i < count - 3);
    }

    for (; i < count; ++i) {
        float value = context->mapped[channel][i] + delta[i];
        if (value > context->state->maximum) value = context->state->maximum;
        if (!(value > bias + source[i])) value = bias + source[i];
        output[i] = value;
    }
}
