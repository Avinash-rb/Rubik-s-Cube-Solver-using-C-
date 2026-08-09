#pragma once
#ifndef EDGEDATABASE
#define EDGEDATABASE

#include "../../Models/ThreeDArrayModel.cpp"
#include "../FileManager.cpp"
#include "DecodeEdges.hpp"
#include "EncodeEdges.hpp"
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

class EdgeDB {
private:
    string path;
    vector<uint8_t> database;
    int databaseSize = 85155840 / 2;
    
public:
    EdgeDB() {}

    EdgeDB(string path) {
        this->path = path;
        database.assign(databaseSize, 0xFF);
    }

    void setDistance(int index, int distance) {
        if (index & 1) {
            database[index / 2] = (database[index / 2] & 15) | (distance << 4);
        } else {
            database[index / 2] = (database[index / 2] & 240) | distance;
        }
    }

    int getDistance(int index) {
        if (index & 1) {
            return (database[index / 2] >> 4) & 15;
        } else {
            return database[index / 2] & 15;
        }
    }

    void createDatabase(int offset) {
        if (loadDatabase(database, databaseSize, path)) {
            return;
        }

        queue<int> q;
        EdgeDecoder<ThreeDArrayModel> decoder(offset);
        EdgeEncoder<ThreeDArrayModel> encoder(offset);

        ThreeDArrayModel solvedCube;
        int solvedRank = encoder.getRank(solvedCube);
        
        q.push(solvedRank);
        setDistance(solvedRank, 0);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            ThreeDArrayModel& cube = decoder.getCurrentCube(node);
            int currDistance = getDistance(node);

            for (int i = 0; i < 18; ++i) {
                cube.performMove(static_cast<RubiksCube::Move>(i));
                
                int newRank = encoder.getRank(cube);

                if (currDistance + 1 < getDistance(newRank)) {
                    setDistance(newRank, currDistance + 1);
                    q.push(newRank);
                }

                cube.invertMove(static_cast<RubiksCube::Move>(i));
            }
        }
        saveDatabase(database, databaseSize, path);
    }
};

#endif