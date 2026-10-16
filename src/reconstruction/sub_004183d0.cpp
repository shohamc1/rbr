struct Reader004183d0 {
    const unsigned int* data;
    unsigned int offset;
    void sub_004183d0(unsigned int* output);
};

void Reader004183d0::sub_004183d0(unsigned int* output) {
    const unsigned int* source = (const unsigned int*)((const unsigned char*)data + offset);
    const unsigned int first = source[0];
    const unsigned int second = source[1];
    const unsigned int third = source[2];
    offset += 12;
    output[0] = first;
    output[1] = second;
    output[2] = third;
}
