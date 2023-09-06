#ifndef MOD_HPP
#define MOD_HPP

inline int mod(int a, int b) {
    return ((a % b) + b) % b;
}

#endif // MOD_HPP