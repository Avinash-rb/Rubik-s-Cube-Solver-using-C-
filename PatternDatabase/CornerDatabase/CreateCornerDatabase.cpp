#pragma once
#ifndef CORNERDB_H
#define CORNERDB_H

#include "../../Models/ThreeDArrayModel.cpp"
#include "DecodeCorners.hpp"
#include "EncodeCorners.hpp"
#include "../FileManager.cpp"
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class CornerDB {
private:
    string path;
    vector<uint8_t> database;
    int databaseSize = 88179840 / 2;

public:
    CornerDB(){}
    CornerDB(string path) {
        this->path = path;
        database.assign(databaseSize, 0xFF);
    }

    int getDistance(int index) {
        if (index & 1) {
            return (database[index / 2] >> 4) & 0x0F;
        } else {
            return database[index / 2] & 0x0F;
        }
    }

    void setDistance(int index, int dist) {
        if (index & 1) {
            database[index / 2] = (database[index / 2] & 0x0F) | (dist << 4);
        } else {
            database[index / 2] = (database[index / 2] & 0xF0) | dist;
        }
    }

    void createDatabase() {
        if (loadDatabase(database,databaseSize,path)) {
            return;
        }

        cout << "Database not found. Starting BFS..." << endl;

        queue<int> q;
        CornerDecoder<ThreeDArrayModel> decoder;
        CornerEncoder<ThreeDArrayModel> encoder;

        setDistance(0, 0);
        q.push(0);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            // cout << node << endl;

            ThreeDArrayModel& baseCube = decoder.getCurrentCube(node);
            int currDist = getDistance(node);

            // cout << "hello\n";

            for (int i = 0; i < 18; ++i) {
                baseCube.performMove(static_cast<RubiksCube::Move>(i));
                
                int nextIndex = encoder.getRank(baseCube);

                // cout << nextIndex << endl;

                if (getDistance(nextIndex) == 0x0F) {
                    setDistance(nextIndex, currDist + 1);
                    q.push(nextIndex);
                }

                baseCube.invertMove(static_cast<RubiksCube::Move>(i));
            }
        }

        saveDatabase(database,databaseSize,path);
    }
};

#endif