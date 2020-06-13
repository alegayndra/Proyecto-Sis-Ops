// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//Marco de página
struct ProcesoReal {
    int idProceso;
    double timestamp;
};

//Swaping [Virtual]
struct ProcesoVirtual {
    int idProceso;
    int pagina;
    int marcoDePagina;
    double timestamp;
};

//Proceso
struct Proceso {
    int idProceso;
    double tiempoInicio;
    double tiempoFinal;
};

// Vectores de memoria;
vector<ProcesoReal*> M;
vector<ProcesoVirtual*> S;

// Variables globales
double tiempo;
int tamPagina;

void valoresIniciales() {
    for (int i = 0; i < 2048; i++) {
        M.push_back(NULL);
        S.push_back(NULL);
        S.push_back(NULL);
    }

    tamPagina = 16;
    tiempo = 0;
}

bool parsearInput(string linea) {
    stringstream ss;
    char c;
    bool valor = false;

    ss << linea;
    ss >> c;

    switch (c) {
    case 'P':
        break;
    case 'A':
        break;
    case 'L':
        break;
    case 'C':
        break;
    case 'F':
        break;
    case 'E':
        valor = true;
        break;
    default:
        break;
    }

    return valor;
}

int main() {

    valoresIniciales();

}
