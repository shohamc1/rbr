struct Pair {
    float x, y;
    Pair(float a, float b);
};
static_assert(sizeof(Pair) == 8, "layout");
Pair::Pair(float a, float b) : x(a), y(b) {}
