#ifndef FILE_MANAGER
#define FILE_MANAGER

#include<iostream>
#include<vector>
#include<fstream>
#include<cstdint>
using namespace std;

bool loadDatabase(vector<uint8_t> &database, int databaseSize , string &path) {
        ifstream file(path, ios::in | ios::binary);
        
        if (!file.is_open()) {
            return false; 
        }
         
        cout << "Pattern Database found! Loading from " << path << "..." << endl;
        file.read(reinterpret_cast<char*>(database.data()), databaseSize);
        file.close();

        cout << "Database loaded successfully!" << endl;
        return true;
}

void saveDatabase(vector<uint8_t> &database , int databaseSize, string &path) {
        ofstream file(path, ios::out | ios::binary);
        if (!file) {
            cout << "Error: Could not open file at " << path << endl;
            return;
        }

        file.write(reinterpret_cast<const char*>(database.data()), databaseSize);
        file.close();
        cout << "Pattern Database successfully saved to: " << path << endl;
    }

#endif