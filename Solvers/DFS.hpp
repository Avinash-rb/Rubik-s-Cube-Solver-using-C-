#ifndef DFS_SOLVER
#define DFS_SOLVER

#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../Utils/GenericRubiksHash.hpp"
#include "../Models/RubiksCube.hpp"
using namespace std;

template <class T>
class Dfs {
public:
    T cube;
    unordered_map<T, RubiksCube::Move, GenericHash> visited;
    Dfs(T cube) {
        this->cube = cube;
    }

    bool dfs(T &cube, int depth , int limit){

        if(cube.isSolved()){
            return true;
        }

        if(depth >= limit){
            return false;
        }

        bool res = false;
        for(int i = 0; i < 18; ++i){
            cube.performMove(static_cast<RubiksCube::Move>(i));

            if(visited.find(cube) == visited.end()){
                visited[cube] = static_cast<RubiksCube::Move>(i);
                res = res | dfs(cube,depth+1,limit);
                if(res){
                    cube.invertMove(static_cast<RubiksCube::Move>(i));
                    return res;
                }
                visited.erase(cube);
            }
            cube.invertMove(static_cast<RubiksCube::Move>(i));
        }
        
        return res;
    }

    void solve(int depth) {
        if (this->cube.isSolved()) {
            cout << "Cube is already solved!\n";
            return;
        }

        visited[this->cube] = RubiksCube::Move::stop;
        
        if(dfs(cube , 0 , depth)==true){
            printMove();
        } else {
            cout << "Provided depth is not sufficient\n";
        }

        visited.clear();
    }

    void printMove() {
        vector<RubiksCube::Move> moves;
        T tempCube; 

        while (visited[tempCube] != RubiksCube::Move::stop) {
            moves.push_back(visited[tempCube]);
            tempCube.invertMove(visited[tempCube]);
        }

        reverse(moves.begin(), moves.end());
        
        tempCube.print(); 
        for (auto &move : moves) {
            tempCube.performMove(move);
            tempCube.print();
        }
    }
};

#endif