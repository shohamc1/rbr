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

#if defined(_MSC_VER)
#define OFFSET(type, member) ((unsigned int)&(((type*)0)->member))
#else
#define OFFSET(type, member) __builtin_offsetof(type, member)
#endif
typedef char state_bias_offset[(OFFSET(State006de8a0, bias) == 0x0c) ? 1 : -1];
typedef char state_maximum_offset[(OFFSET(State006de8a0, maximum) == 0x6c) ? 1 : -1];
#if defined(_M_IX86) || defined(__i386__)
typedef char context_state_offset[(OFFSET(Context006de8a0, state) == 4) ? 1 : -1];
typedef char context_mapped_offset[(OFFSET(Context006de8a0, mapped) == 0x0c) ? 1 : -1];
#endif
#undef OFFSET

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
