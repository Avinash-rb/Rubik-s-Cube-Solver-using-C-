#ifndef BFS_SOLVER
#define BFS_SOLVER

#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../Utils/GenericRubiksHash.hpp"
#include "../Models/RubiksCube.hpp"

using namespace std;

template <class T>
class Bfs {
private:
    T cube;
    queue<T> q;
    unordered_map<T, RubiksCube::Move, GenericHash> visited;

public:
    Bfs(T cube) {
        this->cube = cube;
    }

    void solve() {
        if (this->cube.isSolved()) {
            cout << "Cube is already solved!\n";
            return;
        }

        q.push(this->cube);
        visited[this->cube] = RubiksCube::Move::stop;

        while (!q.empty()) {
            T node = q.front();
            q.pop();

            bool flag = false;
            for (int i = 0; i < 18; ++i) {
                node.performMove(static_cast<RubiksCube::Move>(i));
                
                if (visited.find(node) == visited.end()) {
                    visited[node] = static_cast<RubiksCube::Move>(i);
                    
                    if (node.isSolved()) {
                        flag = true;
                        break;
                    }
                    q.push(node);
                }
                node.invertMove(static_cast<RubiksCube::Move>(i));
            }
            if (flag) {
                break;
            }
        }

        printMove();
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