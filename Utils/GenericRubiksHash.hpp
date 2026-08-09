#ifndef GENERICHASH_HPP
#define GENERICHASH_HPP

#include <chrono>
#include "../Models/RubiksCube.hpp"

class GenericHash {
    static uint64_t rng(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
public:
    template <class T>
    size_t operator()(const T& c) const {
        static const uint64_t t = std::chrono::steady_clock::now().time_since_epoch().count();
        uint64_t h = 0;
        for(int f = 0; f < 6; ++f) {
            for(int i = 0; i < 3; ++i) {
                for(int j = 0; j < 3; ++j) {
                    h = h * 31 + RubiksCube::getNumber(c.getColor(f, i, j));
                }
            }
        }
        return rng(h + t);
    }
};

#endif