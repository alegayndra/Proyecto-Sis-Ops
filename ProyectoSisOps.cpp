// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tgmath.h> 

using namespace std;

//Marco de p�gina
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
    int cantPaginas;
};

// Vectores de memoria;
vector<ProcesoReal*> M;
vector<ProcesoVirtual*> S;

vector<Proceso> procesos;

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

void reiniciarValores() {
    M.clear();
    S.clear();
    procesos.clear();

    valoresIniciales();
}

void cargarAMemoria(int proceso, int bytes) {

    int cantPaginas;
    cantPaginas = ceil(bytes / cantPaginas);

    Proceso proc;

    for (int i = 0; i < cantPaginas; i++) {
        ProcesoVirtual virt;

        virt.idProceso = proceso;
        virt.pagina = i + 1;
        virt.timestamp = ++tiempo;

        // conseguir marco de pagina
    }

    proc.idProceso = proceso;
    proc.cantPaginas = cantPaginas;
    proc.tiempoInicio = tiempo;
}

bool parsearInput(string linea) {
    stringstream ss;
    string extra;
    char c;
    bool valor = true; // para saber si seguir el programa o no

    ss << linea;
    ss >> c;

    if (c != 'C') {
        cout << linea << endl;
    }

    switch (c) {
    case 'P':
        int proceso, bytes;
        cargarAMemoria(proceso, bytes);
        break;
    case 'A':
        break;
    case 'L':
        break;
    case 'C':
        getline(ss, extra);
        cout << extra << endl;
        break;
    case 'F':

        // hacer todo el rollo

        reiniciarValores();
        break;
    case 'E':
        valor = false;
        break;
    default:
        break;
    }

    return valor;
}

int main() {

    ifstream entrada;
    string linea;
    bool seguir;

    valoresIniciales();

    entrada.open("");

    if (entrada.is_open()) {
        do {
            getline(entrada, linea);
            seguir = parsearInput(linea);
        } while (seguir);
    }

    entrada.close();
    
    return 0;
}
