extern "C" void sub_0043d500(unsigned int* destination, unsigned int* end,
                             const unsigned int* source) {
    while (destination != end) {
        destination[0] = source[0];
        destination[1] = source[1];
        destination += 2;
    }
}
