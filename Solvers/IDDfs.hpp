#ifndef IDDfs_SOLVER
#define IDDfs_SOLVER

#include "Dfs.hpp"

template <class T>
class IDDfs {
private:
    T cube;
    int depth;
public:
    IDDfs(T cube, int depth) {
        this->cube = cube;
        this -> depth = depth;
    }

    void solve() {
        if (this->cube.isSolved()) {
            cout << "Cube is already solved!\n";
            return;
        }

        Dfs solver(cube);
        for(int i = 1; i <= depth; ++i){
			solver.visited[this->cube] = RubiksCube::Move::stop;
            if(solver.dfs(cube,0,i)){
                solver.printMove();
                return ;
            }
            solver.visited.clear();
        }

        cout << "Provided depth is not sufficient\n";
    }
};

#endif