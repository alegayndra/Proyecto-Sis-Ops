// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

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

<<<<<<< HEAD
int main() {
<<<<<<< HEAD
    
=======

=======
// Vectores de memoria;
vector<ProcesoReal*> M;
vector<ProcesoVirtual*> S;
>>>>>>> refs/remotes/origin/dev

// Variables globales
double tiempo;
int tamPagina;

<<<<<<< HEAD
>>>>>>> ac700f6b2992b714967f3c75f3716d9d477ee8df
=======
void valoresIniciales() {
>>>>>>> refs/remotes/origin/dev
    for (int i = 0; i < 2048; i++) {
        M.push_back(NULL);
        S.push_back(NULL);
        S.push_back(NULL);
    }

    tamPagina = 16;
    tiempo = 0;
}

int main() {

    valoresIniciales();

}
