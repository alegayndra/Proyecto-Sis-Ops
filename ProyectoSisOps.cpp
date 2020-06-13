// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;

//Marco de p�gina
struct ProcesoReal
{
    int idProceso;
    double timestamp;
};

//Swaping [Virtual]
struct ProcesoVirtual
{
    int idProceso;
    int pagina;
    int marcoDePagina;
    double timestamp;
};

//Proceso
struct Proceso
{
    int idProceso;
    double tiempoInicio;
    double tiempoFinal;
};

int main()
{

    // Vectores de memoria;
    vector<ProcesoReal *> M;
    vector<ProcesoVirtual *> S;

    // Variables globales
    double tiempo;
    int tamPagina;

    void valoresIniciales()
    {
        for (int i = 0; i < 2048; i++)
        {
            M.push_back(NULL);
            S.push_back(NULL);
            S.push_back(NULL);
        }

        tamPagina = 16;
        tiempo = 0;
    }

    int main()
    {

        valoresIniciales();
    }
