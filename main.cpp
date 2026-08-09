#include"Models/ThreeDArrayModel.cpp"
#include"Models/OneDArrayModel.cpp"
#include"Models/BitBoardModel.cpp"
#include"Solvers/Bfs.hpp"
#include"Solvers/Dfs.hpp"
#include"Solvers/IDDfs.hpp"
#include<chrono>
#include"PatternDatabase/CornerDatabase/CreateCornerDatabase.cpp"
#include"Solvers/AStar.hpp"
#include"PatternDatabase/EdgeDatabase/CreateEdgeDatabase.cpp"
#include<thread>
using namespace std::chrono;

int main(){
	auto start = high_resolution_clock::now();
    // ThreeDArrayModel cube;
    // OneDArrayModel cube;
    ThreeDArrayModel cube;
    
    cube.scrambleCube(13);

    // Bfs solver(cube); 
    // solver.solve();

    string dbPathCorner = "PatternDatabase/Database/corner.pdb";
    string dbPathEdge1 = "PatternDatabase/Database/edge1.pdb";
    string dbPathEdge2 = "PatternDatabase/Database/edge2.pdb";
    
    CornerDB cornerDB(dbPathCorner);
    cornerDB.createDatabase();

    EdgeDB edgedb1(dbPathEdge1);
    EdgeDB edgedb2(dbPathEdge2);

    cout << "Firing up multi-core generation for Edges..." << endl;

    thread t1(&EdgeDB::createDatabase, &edgedb1, 0); // Offset 0
    thread t2(&EdgeDB::createDatabase, &edgedb2, 6); // Offset 6

    t1.join();
    t2.join();

    AStar solver(cube,cornerDB,edgedb1,edgedb2);
    solver.solve();
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Execution Time: " << duration.count() << " ms\n";
    return 0;
}