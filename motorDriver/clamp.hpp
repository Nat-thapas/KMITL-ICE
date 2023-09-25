int clamp(int n, int l, int u) {
    if (n < l) return l;
    if (n > u) return u;
    return n;
}

float clamp(float n, float l, float u) {
    if (n < l) return l;
    if (n > u) return u;
    return n;
}