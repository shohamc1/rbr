struct Pair {
    float x, y;
    Pair(float a, float b);
};
typedef char SizeCheck[sizeof(Pair) == 8 ? 1 : -1];
Pair::Pair(float a, float b) : x(a), y(b) {}
