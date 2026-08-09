#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
using namespace std;

template<class T>
class CornerEncoder {
public:
    int getRank(T& cube) {         
        auto &[permutation, orientation] = cube.getCornerPermutationAndOrientation();
        // cout << "hello\n";
        int permutationRank = getPermutationRank(permutation);
        int orientationRank = getOrientationRank(orientation);

        return (permutationRank * 2187) + orientationRank;
    }

    int getPermutationRank(vector<int>& permutation) {
        int rank = 0;
        int fact[8];
        fact[0] = fact[1] = 1;
        for (int i = 2; i < 8; ++i) {
            fact[i] = (fact[i - 1] * i);
        }
        
        for (int i = 0; i < 8; ++i) {
            int lessThanMe = 0;
            for (int j = i + 1; j < 8; ++j) {
                lessThanMe += (permutation[j] < permutation[i]);
            }
            rank += lessThanMe * fact[7 - i];
        }

        return rank;
    }

    int getOrientationRank(vector<int>& orientation) {
        int rank = 0;
        for (int i = 0; i < 7; ++i) { 
            rank = rank * 3 + orientation[i];
        }
        return rank;
    }
};



#endif