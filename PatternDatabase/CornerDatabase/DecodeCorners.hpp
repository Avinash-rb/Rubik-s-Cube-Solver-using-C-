#ifndef DECODER_H
#define DECODER_H

#include <vector>
using namespace std;

template<class T>
class CornerDecoder {
public:
    T *cube;
    
    CornerDecoder() {
        cube = new T();
    }

    ~CornerDecoder() {
        delete cube;
    }
    
    vector<int> getCornerOrientation(int rank) {
        int orientationRank = rank % 2187;

        int sum = 0;
        vector<int> twist(8);
        for (int i = 6; i >= 0; --i) {
            int orientation = orientationRank % 3;
            twist[i] = orientation;
            orientationRank /= 3;
            sum += orientation;
        }

        twist[7] = (3 - (sum % 3)) % 3;

        return twist;
    }

    vector<int> getCornerPermutation(int rank) {
        int permRank = rank / 2187;
        vector<int> permutation, fact(8, 0);
        vector<int> sequence = {0, 1, 2, 3, 4, 5, 6, 7};
        fact[0] = 1;
        for (int i = 1; i < 8; ++i) {
            fact[i] = (fact[i - 1] * i);
        }

        for (int i = 7; i >= 0; --i) {
            int num = permRank / fact[i];
            permutation.push_back(sequence[num]);
            sequence.erase(sequence.begin() + num);
            permRank %= fact[i];
        }

        return permutation;
    }

    T& getCurrentCube(int rank) {
        vector<int> permutation = getCornerPermutation(rank);
        vector<int> orientation = getCornerOrientation(rank);

        for (int i = 0; i < 8; ++i) {
            cube->setCorner(i, permutation[i], orientation[i]);
        }

        return *cube; 
    }
};

#endif