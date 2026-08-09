#ifndef RubiksCube_hpp
#define RubiksCube_hpp

#include<iostream>
#include<vector>
#include<string>

using namespace std;

class RubiksCube{
  public:

   static constexpr char face[6] = {'W','G','R','B','O','Y'};

       static constexpr int cornerMap[8][3][3] = {
        {{0, 2, 0}, {1, 0, 2}, {2, 0, 0}}, // 0: White-Green-Red (Top-Left-Front)
        {{0, 2, 2}, {2, 0, 2}, {3, 0, 0}}, // 1: White-Red-Blue (Top-Front-Right)
        {{2, 2, 0}, {1, 2, 2}, {5, 0, 0}}, // 2: Red-Green-Yellow (Front-Left-Bottom)
        {{2, 2, 2}, {5, 0, 2}, {3, 2, 0}}, // 3: Red-Yellow-Blue (Front-Bottom-Right)
        {{0, 0, 2}, {3, 0, 2}, {4, 0, 0}}, // 4: White-Blue-Orange (Top-Right-Back)
        {{0, 0, 0}, {4, 0, 2}, {1, 0, 0}}, // 5: White-Orange-Green (Top-Back-Left)
        {{4, 2, 0}, {3, 2, 2}, {5, 2, 2}}, // 6: Orange-Blue-Yellow (Back-Right-Bottom)
        {{4, 2, 2}, {5, 2, 0}, {1, 2, 0}}  // 7: Orange-Yellow-Green (Back-Bottom-Left)
    };

     // cornerOrientations[CornerID][Twist] -> {Color0, Color1, Color2}
    static constexpr int cornerOrientations[8][3][3] = {
        {{0, 1, 2}, {2, 0, 1}, {1, 2, 0}}, // 0: White, Green, Red
        {{0, 2, 3}, {3, 0, 2}, {2, 3, 0}}, // 1: White, Red, Blue
        {{2, 1, 5}, {5, 2, 1}, {1, 5, 2}}, // 2: Red, Green, Yellow
        {{2, 5, 3}, {3, 2, 5}, {5, 3, 2}}, // 3: Red, Yellow, Blue
        {{0, 3, 4}, {4, 0, 3}, {3, 4, 0}}, // 4: White, Blue, Orange
        {{0, 4, 1}, {1, 0, 4}, {4, 1, 0}}, // 5: White, Orange, Green
        {{4, 3, 5}, {5, 4, 3}, {3, 5, 4}}, // 6: Orange, Blue, Yellow
        {{4, 5, 1}, {1, 4, 5}, {5, 1, 4}}  // 7: Orange, Yellow, Green
    };

    static constexpr int edgeMap[12][2][3] = {
        // --- TOP LAYER ---
        {{0, 0, 1}, {4, 0, 1}}, // 0: UB (Up-Back)
        {{0, 1, 2}, {3, 0, 1}}, // 1: UR (Up-Right)
        {{0, 2, 1}, {2, 0, 1}}, // 2: UF (Up-Front)
        {{0, 1, 0}, {1, 0, 1}}, // 3: UL (Up-Left)

        // --- MIDDLE LAYER ---
        {{4, 1, 0}, {3, 1, 2}}, // 4: BR (Back-Right)
        {{2, 1, 2}, {3, 1, 0}}, // 5: FR (Front-Right)
        {{2, 1, 0}, {1, 1, 2}}, // 6: FL (Front-Left)
        {{4, 1, 2}, {1, 1, 0}}, // 7: BL (Back-Left)

        // --- BOTTOM LAYER ---
        {{5, 2, 1}, {4, 2, 1}}, // 8: DB (Down-Back)
        {{5, 1, 2}, {3, 2, 1}}, // 9: DR (Down-Right)
        {{5, 0, 1}, {2, 2, 1}}, // 10: DF (Down-Front)
        {{5, 1, 0}, {1, 2, 1}}  // 11: DL (Down-Left)
    };

    static constexpr int edgeOrientation[12][2][2] = {
        // --- TOP LAYER ---
        {{0, 4}, {4, 0}}, // 0: UB 
        {{0, 3}, {3, 0}}, // 1: UR 
        {{0, 2}, {2, 0}}, // 2: UF 
        {{0, 1}, {1, 0}}, // 3: UL 

        // --- MIDDLE LAYER ---
        {{4, 3}, {3, 4}}, // 4: BR 
        {{2, 3}, {3, 2}}, // 5: FR 
        {{2, 1}, {1, 2}}, // 6: FL 
        {{4, 1}, {1, 4}}, // 7: BL 

        // --- BOTTOM LAYER ---
        {{5, 4}, {4, 5}}, // 8: DB 
        {{5, 3}, {3, 5}}, // 9: DR 
        {{5, 2}, {2, 5}}, // 10: DF 
        {{5, 1}, {1, 5}}  // 11: DL 
    };

    pair<vector<int>,vector<int>> res = {vector<int>(8),vector<int>(8)};

    vector<int> cornerNo = {7, 13, 38, 44, 25, 19, 56, 50};

    vector<int> edgeNo = {17 , 9 , 5 , 3 , 24 , 12 , 6 , 18 , 48 , 40 , 36 , 34};

    enum class Move{
        U, UPRIME, U2,
        D, DPRIME, D2,
        L, LPRIME, L2,
        R ,RPRIME, R2,
        F, FPRIME, F2,
        B, BPRIME, B2,
        stop
    };

    static int getNumber(char color);
    
    void printPartial(int face);

    void print();
    
    bool isSolved();

    string getMove(int num);

    void performMove(Move move);

    void invertMove(Move move);

    void scrambleCube(int num);

    pair<vector<int>,vector<int>>& getCornerPermutationAndOrientation();

    pair<vector<int>,vector<int>>& getEdgePermutationAndOrientation();

    virtual char getColor(int face, int row, int col) const = 0;

    virtual void rotateFace(int face) = 0;

    virtual void L() = 0;
    virtual void L2() = 0;
    virtual void Lprime() = 0;

    virtual void R() = 0;
    virtual void R2() = 0;
    virtual void Rprime() = 0;

    virtual void F() = 0;
    virtual void F2() = 0;
    virtual void Fprime() = 0;

    virtual void B() = 0;
    virtual void B2() = 0;
    virtual void Bprime() = 0;

    virtual void U() = 0;
    virtual void U2() = 0; 
    virtual void Uprime() = 0;

    virtual void D() = 0;
    virtual void D2() = 0;
    virtual void Dprime() = 0;
    
    virtual void setCorner(int cornerNo, int actualCorner, int orientation) = 0;

    virtual void setEdge(int edgeNo, int actualEdge, int orientation) = 0;

};
#endif























