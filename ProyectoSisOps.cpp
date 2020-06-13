// ProyectoSisOps.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Librerías auxiliares
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
    int cantPageFaults;
};

// Vectores de memoria;
vector<ProcesoReal*> M;
vector<ProcesoVirtual*> S;

//Vector de procesos
vector<Proceso> procesos;

// Variables globales
double tiempo;
int tamPagina;
int cantSwaps;
string politica;

/*Funcion donde se inicializa como NULL los vectores (ya que son de apuntadores) */
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

/*Función utilizada para reiniciar los valores en la función de "fin" */
void reiniciarValores() {
    M.clear();
    S.clear();
    procesos.clear();

    valoresIniciales();
}

void swapping(int posicion) {
    swapping++;
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

/*Función utilizada para cargar un procesos a memoria
    Parámetros:
        -bytes: representa el tamaño del proceso
        -proceso: representa el ID del proceso
 */
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

/*Funcion utilizada para liberar el proceso de memoria
    Parámetros:
        -Proceso: Representa el ID del proceso a liberar
*/
void liberarProceso(int proceso) {
    //Utilizado para registrar los marcos de pag. a liberar
    vector <int> marcosDePagina;

    //Utilizado para registrar las pag. a liberar
    vector <int> paginas;

    //Ciclo para liberar los marcos de página ocupados por el proceso en la memoria real
    for(int i = 0; i < 128; i++){
        if(proceso == M[i]->idProceso){
            marcosDePagina.push_back(i);
            tiempo += 0.1;
            delete M[i];
            M[i] = NULL;
        }
    }

    //Ciclo para liberar las páginas ocupadas por el proceso en la memoria virtual     
    for(int i = 0; i < 256; i++){
        if(proceso == S[i]->idProceso){
            paginas.push_back(i);
            tiempo += 0.1;
            delete S[i];
            S[i] = NULL;
        }
    }

    //Ciclo para buscar el proceso que se acaba de liberar y marcar que ya terminó  
    for(int i = 0; i < procesos.size(); i++){
        if(proceso == procesos[i].idProceso){
            procesos[i].tiempoFinal = tiempo;
        }
    }
}

void finCiclo() {
    int turnaroundProm = 0;

    //El tiempo final es de -1
    for(int i = 0; i < procesos.size(); i++){
        if(procesos[i].tiempoFinal == -1){
            procesos[i].tiempoFinal = tiempo;
        }
        cout << "Turnaround del proceso " << procesos[i].idProceso << " es = " << procesos[i].tiempoFinal - procesos[i].tiempoInicio << endl;
        turnaroundProm += (procesos[i].tiempoFinal - procesos[i].tiempoInicio);
    }
    turnaroundProm /= procesos.size();
    cout << "Turnaround promedio es = " << turnaroundProm << endl;
    for(int i = 0; < procesos.size(); i++){
        cout << "La cantidad de Page Faults del proceso " << procesos[i].idProceso << " es = " << procesos[i].cantPageFaults << endl;
    }
    cout << "La cantidad de swaps es = " << cantSwaps << endl;
}

/*Funcion utilizada para procesar la entrada, y saber que instrucción se quiere ejecutar
    Parámetros:
        -línea: La instrucción a procesar
    Output:
        -bool: Especifíca si ya se terminó el proceso
*/
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
        //Suena dubstep en el fondo
        cout << "aZtaaaa la procSimaaaaaa" << endl;

        valor = false;
        break;
    default:
        break;
    }

    return valor;
}

/*Funcion principal para la lectura del archivo y reinicio del proceso*/
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
