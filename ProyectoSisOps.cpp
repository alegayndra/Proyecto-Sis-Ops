// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tgmath.h> 

using namespace std;

//Marco de pagina
struct ProcesoReal {
    int idProceso;
    double timestamp;
    int cantBytes;
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
vector<ProcesoVirtual> S;

vector<Proceso> procesos;

// Variables globales
double tiempo;
int tamPagina;

void valoresIniciales() {
    for (int i = 0; i < 128; i++) {
        M.push_back(NULL);
    }

    tiempo = 0;
    tamPagina = 16;
}

void reiniciarValores() {
    M.clear();
    S.clear();
    procesos.clear();

    valoresIniciales();
}

void cargarAMemoria(int bytes, int proceso) {

    int cantPaginas;
    cantPaginas = ceil(bytes / cantPaginas);

    if (cantPaginas + S.size() <= 2048) {
        Proceso proc;

        for (int i = 0; i < cantPaginas; i++) {
            ProcesoVirtual virt;

            virt.idProceso = proceso;
            virt.pagina = i;
            virt.timestamp = ++tiempo;

            // conseguir marco de pagina
        }

        proc.idProceso = proceso;
        proc.cantPaginas = cantPaginas;
        proc.tiempoInicio = tiempo;
    }
}

void accederADireccion(int direccion, int proceso, bool modificar) {

}

void liberarProceso(int proceso) {
    //Utilizado para registrar los marcos de pag. a liberar
    vector <int> marcosDePagina;
    //Utilizado para registrar las pag. a liberar
    vector <int> paginas;
    for(int i = 0; i < 128; i++){
        if(proceso == M[i]->idProceso){
            marcosDePagina.push_back(i);
            tiempo += 0.1;
            delete M[i];
            M[i] = NULL;
        }
    }
    
    for(int i = 0; i < 256; i++){
        if(proceso == S[i]->idProceso){
            paginas.push_back(i);
            tiempo += 0.1;
            delete S[i];
            S[i] = NULL;
        }
    }
}

void finCiclo() {

}

bool parsearInput(string linea) {
    stringstream ss;
    string extra;
    string n, p, d, m;
    char c;
    bool valor = true; // para saber si seguir el programa o no

    ss << linea;
    ss >> c;

    if (c != 'C') {
        cout << linea << endl;
    }

    switch (c) {
    case 'P':
        ss >> n >> p;
        cargarAMemoria(stoi(n), stoi(p));
        break;
    case 'A':
        ss >> d >> p >> m;
        accederADireccion(stoi(d), stoi(p), stoi(m));
        break;
    case 'L':
        ss >> p;
        liberarProceso(stoi(p));
        break;
    case 'C':
        getline(ss, extra);
        cout << extra << endl;
        break;
    case 'F':
        finCiclo();
        reiniciarValores();
        break;
    case 'E':

        // mensaje de despedida

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
