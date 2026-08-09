#ifndef EDGEENCODER
#define EDGEENCODER 

#include <vector>

using namespace std;

template<class T>
class EdgeEncoder {
public:
    int offset;
    int waysToPlaceRemaining[6] = { 55440, 5040, 504, 56, 7, 1 };

    EdgeEncoder(int offset) : offset(offset){}

    int getRank(T &cube){
        auto& [permutation, orientation] = cube.getEdgePermutationAndOrientation();
        int permutationRank = getPermutationRank(permutation);
        int orientationRank = getOrientationRank(permutation, orientation);

        int finalRank = (permutationRank * 64) + orientationRank;
        if(offset == 6){
            finalRank += 42577920;
        }

        return finalRank;
    }  

    int getPermutationRank(vector<int> &permutation){
        vector<bool> placed(12, false);
        int permRank = 0;

        for(int i = 0; i < 6; ++i){
            int target = i + offset;
            int empty = 0;
            
            for(int j = 0; j < 12; ++j){
                if(permutation[j] == target) {
                    placed[j] = true;
                    break;
                }
                if(!placed[j]) {
                    ++empty;
                }
            }
            permRank += empty * waysToPlaceRemaining[i];
        }

        return permRank;
    }

    int getOrientationRank(vector<int> &permutation, vector<int> &orientation){
        int oriRank = 0;
        
        for(int i = 0; i < 6; ++i){
            int target = i + offset;
            for(int j = 0; j < 12; ++j){
                if(permutation[j] == target){
                    oriRank = (oriRank * 2) + orientation[j];
                    break;
                }
            }
        }
        return oriRank;
    }
};

#endif