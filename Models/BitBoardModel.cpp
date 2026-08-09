#pragma once
#ifndef BITBOARDMODEL_CPP
#define BITBOARDMODEL_CPP

#include<RubiksCube.hpp>
#include<cstdint>

class BitBoardModel : public RubiksCube{
    private:
        uint64_t cube[6];
    public:
        BitBoardModel(){
            for(int i =0; i < 6; i++){
                cube[i] = 0;
                for(int j =i; j < 64; j += 8){
                    this->cube[i] |= (1ULL << j);
                }
            }
        }
        
        int getNo(int num){
            if(num == 4) 
               return -1;
            return num < 4 ? num : --num;
        }
        
        char getColor(int face, int row, int col) const override {
            int cell = (row * 3) + col;
            if(cell == 4) return RubiksCube::face[face];
            if(cell > 4) cell--;


            uint64_t b = (this->cube[face] >> (cell * 8)) & 0xFFULL;
            return RubiksCube::face[__builtin_ctzll(b)];
        }
        uint64_t getMask(uint64_t num, int no){
            return (num >> (no * 8)) & 0xFFULL;
        }

        void setMask(uint64_t &num, uint64_t mask, int no){
            num &= (~(0xFFULL << (no * 8)));
            num |= (mask << (no * 8));
        }
        
        void rotateFace(int face) override{
            uint64_t temp = this->cube[face];
            this->cube[face] = 0;

            setMask(cube[face], getMask(temp, 5),0);
            setMask(cube[face], getMask(temp, 3),1);
            setMask(cube[face], getMask(temp, 0),2);
            setMask(cube[face], getMask(temp, 6),3);
            setMask(cube[face], getMask(temp, 1),4);
            setMask(cube[face], getMask(temp, 7),5);
            setMask(cube[face], getMask(temp, 4),6);
            setMask(cube[face], getMask(temp, 2),7);
        }

        void L() override {
            this->rotateFace(1);
            uint64_t temp = 0;

            for(int i = 0; i < 3; i++){
                setMask(temp, getMask(cube[0], getNo(3 * i)), i );
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[0], getMask(cube[4], getNo(8 - 3 * i)), getNo(3 * i));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[4],getMask(cube[5], getNo(3 * abs(i - 2))), getNo(3 * i + 2));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[5], getMask(cube[2], getNo(3 * i)), getNo(3 * i));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[2], getMask(temp,i), getNo(3 * i));
            }

        }

        void L2() override {
            this->L();
            this->L();
        }

        void Lprime() override {
            this->L2();
            this->L();
        }

        void R() override {
            this->rotateFace(3);
            uint64_t temp = 0;

            for(int i = 0; i < 3; i++){
                setMask(temp, getMask(cube[0], getNo(3 * i + 2)), i);

            }
            for(int i = 0; i < 3; i++){
                setMask(cube[0], getMask(cube[2], getNo(3 * i + 2)), getNo(3 * i + 2));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[2], getMask(cube[5], getNo(3 * i + 2)), getNo(3 * i + 2));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[5], getMask(cube[4], getNo(3 * abs(i - 2))), getNo(3 * i + 2));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[4], getMask(temp,(2 - i)), getNo(3 * i));
            }
        }
        void R2() override {
            this->R();
            this->R();
        }
        void Rprime () override {
            this->R2();
            this->R();
        }

        void U() override {
            this->rotateFace(0);
            uint64_t temp = 0;

            for(int i =0; i < 3; i++){
                setMask(temp, getMask(cube[4], getNo(i)), i);
            }
            for(int i =0; i < 3; i++){
                setMask(cube[4], getMask(cube[1], getNo(i)), getNo(i));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[1], getMask(cube[2], getNo(i)), getNo(i));

            }
            for(int i =0; i < 3; i++){
                setMask(cube[2], getMask(cube[3], getNo(i)), getNo(i));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[3], getMask(temp, i), getNo(i));
            }
        }
        void U2() override {
            this->U();
            this->U();
        }
        void Uprime() override {
            this->U2();
            this->U();
        }

        void D() override {
            this->rotateFace(5);
            uint64_t temp = 0;

            for(int i =0; i < 3; i++){
                setMask(temp, getMask(cube[2], getNo(6 + i)), i);
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[2], getMask(cube[3], getNo(6 + i)), getNo(6 + i));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[3], getMask(cube[4], getNo(6 + i)), getNo(6 + i));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[4], getMask(cube[1],getNo(6 + i)), getNo(6 + i));
            }
            for(int i  =0; i < 3; i++){
                setMask(cube[1], getMask(temp,i), getNo(6 + i));
            }
        }
        void D2() override {
            this->D();
            this->D();
        }
        void Dprime() override {
            this->D2();
            this->D();
        }

        void F() override {
            this->rotateFace(2);
            uint64_t temp = 0;
            
            for(int i =0; i < 3; i++){
                setMask(temp, getMask(cube[0], getNo(6 + i)), i);
            }
            for(int i =0; i < 3; i++){
                setMask(cube[0], getMask(cube[1], getNo(3 * abs(2-i) + 2)), getNo(6 + i));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[1], getMask(cube[5], getNo(i)), getNo(3 * abs(2-i) + 2));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[5], getMask(cube[3], getNo(3 * abs(2 - i))), getNo(i));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[3], getMask(temp, i), getNo(3 * abs(2 - i)));
            }

        }
        void F2() override {
            this->F();
            this->F();
        }
        void Fprime() override {
             this->F2(); 
             this->F();
        }

        void B() override {
            this->rotateFace(4);
            uint64_t temp = 0;

            for(int i =0; i < 3; i++){
                setMask(temp, getMask(cube[0], getNo(i)), i);
            }
            for(int i =0; i < 3; i++){
                setMask(cube[0], getMask(cube[3], getNo(3 * i + 2)), getNo(i));
            }
            for(int i =0; i < 3; i++){
                setMask(cube[3], getMask(cube[5], getNo(8 - i)), getNo(3 * i + 2));
            }
            for(int i = 0; i < 3; i++){
                setMask(cube[5], getMask(cube[1], getNo(3 * i)), getNo(6 + i));
            }
            for(int i =0;  i < 3; i++){
                setMask(cube[1], getMask(temp, abs(2-i)), getNo(3 * i));
            }
        }
        void B2() override {
            this->B();
            this->B();
        }
        void Bprime() override {
            this->B2(); 
            this->B();
        }
    


            bool operator==(const BitBoardModel& other) const {
        for (int i = 0; i < 6; ++i) {
            if (this->cube[i] != other.cube[i]) {
                return false;
            }
        }
        return true;
    }
        
};

#endif