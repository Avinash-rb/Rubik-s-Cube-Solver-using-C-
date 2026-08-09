#ifndef EDGEDECODER
#define EDGEDECODER

#include<vector>

template<class T>
class EdgeDecoder{
private:
    T* cube;
    int offset;
    int waysToPlaceRemaining[6] = { 55440, 5040, 504, 56, 7, 1 };
public:
    EdgeDecoder(int offset){ 
        cube = new T();
        this -> offset = offset;
    }

    ~EdgeDecoder(){
        delete cube;
    }

    vector<int> getPermutation(int rank){
        if(offset==6){
            rank -= 42577920;
        }
        rank = rank / 64;
        vector<int> permutation(12);
        vector<bool> placed(12,false);
        for(int i = 0+offset; i < 6+offset; ++i){
             int empty = rank/waysToPlaceRemaining[i-offset];
             rank %= waysToPlaceRemaining[i-offset];
             int cnt=0;
             for(int j = 0; j < 12; ++j){
                if(!placed[j] && cnt == empty){
                    permutation[j] = i;
                    placed[j]=true;
                    break;
                }
                if(!placed[j]){
                    ++cnt;
                }
             }
        }

        int counter=0;
        if(!offset){
            counter=6;
        }
        for(int i = 0; i < 12; ++i){
            if(!placed[i]){
                permutation[i]=counter++;
            }
        }
        return permutation;
    }

    vector<int> getOrientation(int rank,vector<int> &permutation){
        if(offset==6){
            rank -= 42577920;
        }
        rank %= 64;
        vector<int> orientation(12,0);
        for(int i = 5+offset; i >= offset+0; --i){
            int state = rank/(1<<(i-offset));
            rank %= (1 << (i-offset));
            for(int j = 0; j < 12; ++j){
                if(permutation[j] == i){
                    orientation[j]=state;
                    break;
                }
            }
        }
        return orientation;
    }

    T& getCurrentCube(int rank){
        vector<int> permutation = getPermutation(rank);
        vector<int> orientation = getOrientation(rank,permutation);

        for(int i = 0; i < 12; ++i){
            cube -> setEdge(i , permutation[i] , orientation[i]);
        }

        return *cube;
    }
};

#endif