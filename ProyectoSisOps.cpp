// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

vector<bool> M, S;

int main() {

    for (int i = 0; i < 2048; i++) {
        M.push_back(false);
        S.push_back(false);
        S.push_back(false);
    }

    cout << "M size: !" << M.size() << "\n";
    cout << "S size: !" << S.size() <<"\n";
}
