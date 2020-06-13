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
    int cantBytes;
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
int cantPageFaults;
string politica;

void valoresIniciales() {
    for (int i = 0; i < 128; i++) {
        M.push_back(NULL);
        S.push_back(NULL);
        S.push_back(NULL);
    }

    tiempo = 0;
    tamPagina = 16;
    cantPageFaults = 0;
}

void reiniciarValores() {
    M.clear();
    S.clear();
    procesos.clear();

    valoresIniciales();
}

void swapping(int posicion) {

    tiempo += 0.1;

    double valor = INT_MAX;
    int posReal;

    for (int i = 0; i < 128; i++) {
        if (M[i] != NULL && valor > M[i]->timestamp) {
            valor = M[i]->timestamp;
            posReal = i;
        }
    }

    int proceso = M[posReal]->idProceso;

    for (int i = 0; i < 256; i++) {
        if (S[i] != NULL && S[i]->idProceso == proceso && S[i]->marcoDePagina == posReal) {
            S[i]->marcoDePagina = -1;

            S[posicion]->marcoDePagina = posReal;

            M[posReal]->idProceso = S[posicion]->idProceso;
            M[posReal]->timestamp = tiempo;
            M[posReal]->cantBytes = S[posicion]->cantBytes;
        }
    }
}

void cargarAMemoria(int bytes, int proceso) {

    int cantPaginas;
    cantPaginas = ceil(bytes / cantPaginas);

    if (cantPaginas + S.size() <= 2048) {
        Proceso proc;

        for (int i = 0; i < cantPaginas; i++) {
            bool paginaEncontrada = false;

            for (int j = 0; !paginaEncontrada && j < M.size(); j++) {
                if (S[j] == NULL) {
                    S[j] = new ProcesoVirtual;
                    S[j]->idProceso = proceso;
                    S[j]->timestamp = ++tiempo;
                    S[j]->pagina = j;

                    paginaEncontrada = true;

                    // conseguir marco de pagina
                    bool memoriaEncontrada = false;
                    for (int j = 0; !memoriaEncontrada && j < M.size(); j++) {
                        if (M[j] == NULL) {
                            M[j] = new ProcesoReal;
                            M[j]->idProceso = proceso;
                            M[j]->timestamp = tiempo;
                            M[j]->cantBytes = (bytes > tamPagina) ? tamPagina : bytes;

                            S[i]->cantBytes = (bytes > tamPagina) ? tamPagina : bytes;
                            S[i]->marcoDePagina = j;

                            bytes -= tamPagina;
                            
                            memoriaEncontrada = true;
                        }
                    }

                    if (!memoriaEncontrada) {
                        swapping(i);
                    }
                }
            }
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
