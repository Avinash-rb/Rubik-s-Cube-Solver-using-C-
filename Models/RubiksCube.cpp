#include "RubiksCube.hpp"
#include <vector>
#include <random>
#include <chrono>

void RubiksCube::printPartial(int face){
    for(int row =0; row < 3; row++){
        for(int temp = 0; temp < 3; temp++){
            cout<< "  ";
        }
        cout << "  ";
        for(int col = 0; col < 3; col++){
            cout<<getColor(face, row, col)<< " ";
        }
        cout << endl;
    }
    cout << endl;
}

void RubiksCube::print(){
    printPartial(0);
    for(int row = 0; row < 3; row++){
        for(int face = 1; face <= 4; face++){
            for(int col = 0; col < 3; col++){
                cout<<getColor(face, row, col) << " ";
            }
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;

    printPartial(5);
    cout << endl;

}

bool RubiksCube::isSolved(){
    for(int i =0; i  < 6; i++){
        for(int j = 0; j < 3;j++){
            for(int k = 0; k < 3; k++){
                if(getColor(i,j,k) != face[i])
                   return false;
            }
        }
    }
    return true;
}

int RubiksCube::getNumber(char color){
    switch(color){
        case 'W' : return 0;
        case 'G' : return 1;
        case 'R' : return 2;
        case 'B' : return 3;
        case 'O' : return 4;
        case 'Y' : return 5;
        default : return -1;
    }
}

void RubiksCube::performMove(Move move){
    switch(move){
        case Move::U :
            return this->U();
        case Move::U2 :
            return this->U2();
        case Move::UPRIME :
            return this->Uprime();
        case Move::D : 
            return this->D();
        case Move::D2 :
            return this->D2();
        case Move::DPRIME :
            return this->Dprime();
        case Move::L :
            return this->L();
        case Move::L2 :
            return this->L2();
        case Move::LPRIME :
            return this->Lprime();
        case Move::R :
            return this->R();
        case Move::R2 :
            return this->R2();
        case Move::RPRIME :
            return this->Rprime();
        case Move::F :
            return this->F();
        case Move::F2 :
            return this->F2();
        case Move::FPRIME :
            return this->Fprime();
        case Move::B :
            return this->B();
        case Move::B2 :
            return this->B2();
        case Move::BPRIME :
            return this->Bprime();
        default : 
            return;
    }
}

void RubiksCube::invertMove(Move move){
    switch(move){
        case Move::U :
            return this->Uprime();

        case Move::U2 :
            return this->U2();
        case Move::UPRIME :
            return this->U();
        case Move::D :
            return this->Dprime();
        case Move::D2 :
            return this->D2();
        case Move::DPRIME :
            return this->D();
        case Move::L :
            return this->Lprime();
        case Move::L2 :
            return this->L2();
        case Move::LPRIME :
            return this->L();
        case Move::R :
            return this->Rprime();
        case Move::R2 :
            return this->R2();
        case Move::RPRIME :
            return this->R();
        case Move::F :
            return this->Fprime();
        case Move::F2 :
            return this->F2();
        case Move::FPRIME :
            return this->F();
        case Move::B :
            return this->Bprime();
        case Move::B2 :
            return this->B2();
        case Move::BPRIME :
            return this->B();
        default :
            return;
    }
}

void RubiksCube::scrambleCube(int num) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    int prevFace = -1; 
    int movesApplied = 0;
    
    while(movesApplied < num) {
        int randomMove = rng() % 18;
        int currentFace = randomMove / 3;
        
        if(currentFace != prevFace) {
            this->performMove(static_cast<RubiksCube::Move>(randomMove));
            prevFace = currentFace;
            movesApplied++;
        }
    }
}



pair<vector<int>,vector<int>>& RubiksCube::getCornerPermutationAndOrientation() {
    res.first.assign(8, 0);
    res.second.assign(8, 0);
    for (int i = 0; i < 8; i++) {
        char c0 = getColor(cornerMap[i][0][0], cornerMap[i][0][1], cornerMap[i][0][2]);
        char c1 = getColor(cornerMap[i][1][0], cornerMap[i][1][1], cornerMap[i][1][2]);
        char c2 = getColor(cornerMap[i][2][0], cornerMap[i][2][1], cornerMap[i][2][2]);
        
        int n0 = getNumber(c0);
        int n1 = getNumber(c1);
        int n2 = getNumber(c2);
        
        for (int j = 0; j < 8; j++) {
            if (cornerOrientations[j][0][0] == n0 && cornerOrientations[j][0][1] == n1 && cornerOrientations[j][0][2] == n2) {
                res.first[i] = j;
                res.second[i] = 0;
                break;
            }
            if (cornerOrientations[j][1][0] == n0 && cornerOrientations[j][1][1] == n1 && cornerOrientations[j][1][2] == n2) {
                res.first[i] = j;
                res.second[i] = 1;
                break;
            }
            if (cornerOrientations[j][2][0] == n0 && cornerOrientations[j][2][1] == n1 && cornerOrientations[j][2][2] == n2) {
                res.first[i] = j;
                res.second[i] = 2;
                break;
            }
        }
    }
    return res;
}

pair<vector<int>,vector<int>>& RubiksCube::getEdgePermutationAndOrientation() {
    res.first.assign(12, 0);
    res.second.assign(12, 0);
    for (int i = 0; i < 12; i++) {
        char c0 = getColor(edgeMap[i][0][0], edgeMap[i][0][1], edgeMap[i][0][2]);
        char c1 = getColor(edgeMap[i][1][0], edgeMap[i][1][1], edgeMap[i][1][2]);
        
        int n0 = getNumber(c0);
        int n1 = getNumber(c1);
        
        for (int j = 0; j < 12; j++) {
            if (edgeOrientation[j][0][0] == n0 && edgeOrientation[j][0][1] == n1) {
                res.first[i] = j;
                res.second[i] = 0;
                break;
            }
            if (edgeOrientation[j][1][0] == n0 && edgeOrientation[j][1][1] == n1) {
                res.first[i] = j;
                res.second[i] = 1;
                break;
            }
        }
    }
    return res;
}
