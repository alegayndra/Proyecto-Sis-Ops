#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <tgmath.h>
#include <climits> 

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
    int tamProceso;
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
    cantSwaps = 0;
}

/*Función utilizada para reiniciar los valores en la función de "fin" */
void reiniciarValores() {
    M.clear();
    S.clear();
    procesos.clear();

    valoresIniciales();
}

/*
    Función para hacer el reemplazo de páginas
    Parámetros:
    - posicion (int): posicion en S de la página de un proceso a la cual se le quiere asignar algún marco de página
*/
void swapping(int posicion) {

    cantSwaps++;
    tiempo++;

    double valor = INT_MAX;
    int posReal;

    //Buscamos el valor más chico de timestamp
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

            cout << "La pagina " << posReal << " del proceso " << proceso << " fue swappeada al marco " << S[i]->marcoDePagina << endl;
        }
        else{
            cout << "El proceso no cabe en memoria virtual" << endl;
        }
    }
}

/*
    Función utilizada para cargar un procesos a memoria
    Parámetros:
        -bytes: representa el tamaño del proceso
        -proceso: representa el ID del proceso
 */
void cargarAMemoria(int bytes, int proceso) {

    if (bytes <= 2048) {

        int cantPaginasVirtLibres = 0;
        int cantPaginas = ceil(bytes / tamPagina);

        for (int i = 0; i < 256; i++) {
            if (S[i] == NULL) {
                cantPaginasVirtLibres++;
            }
        }

        if (cantPaginasVirtLibres >= cantPaginas) {
            Proceso proc;
            bool paginaEncontrada;
            int bytesExtra = bytes;

            for (int i = 0; i < cantPaginas; i++) {
                paginaEncontrada = false;

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
                                M[j]->cantBytes = (bytesExtra > tamPagina) ? tamPagina : bytesExtra;

                                S[i]->cantBytes = (bytesExtra > tamPagina) ? tamPagina : bytesExtra;
                                S[i]->marcoDePagina = j;

                                /*cout << "\nAsignando valor a marco de pagina\n";
                                cout << "Marco pagina: " << S[i]->marcoDePagina << endl;
                                cout << "j: " << j << endl;
                                cout << "valor asignado\n";*/

                                bytesExtra -= tamPagina;

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
            proc.tamProceso = bytes;
            proc.tiempoFinal = -1;
            proc.cantPageFaults = 0;

            procesos.push_back(proc);

            cout << "Se asignaron los marcos de pagina [ ";
            for (int i = 0; i < 128; i++) {
                if (M[i] != NULL && proceso == M[i]->idProceso) {
                    cout << i << ", ";
                }
            }
            cout << " ]" << endl;
        }
        else {
            cout << "ERROR: No cabe el proceso en memoria virtual\n";
        }
    }
    else {
        cout << "ERROR: No cabe el proceso en memoria real\n";
    }

    
}

/*
    Funcion utilizada para acceder a una direccion de memoria de un proceso dado
    Parámetros:
        -direccion: Direccion virtual
        -proceso: ID del proceso
        -modificar: condicion para saber si fue modificado
*/
void accederADireccion(int direccion, int proceso, bool modificar) {
    cout << "Obtener la direccion real correspondiente a la direccion virtual " << direccion << " del proceso " << proceso << endl;
    tiempo += 0.1;
    for(int i = 0; i < procesos.size(); i++){
        //Recorrer los procesos hasta encontrar el indicado con ID
        if(proceso == procesos[i].idProceso){
            //Calcular pagina
            int pagina = (direccion / tamPagina) - ((direccion % tamPagina == 0 && direccion != 0) ? 1 : 0);

            //Recorrer las paginas hasta que encontremos la pagina donde esta el proceso
            for(int j = 0; j < 256; j++){
                if(S[j] != NULL && S[j]->idProceso == proceso && S[j]->pagina == pagina){

                    //Checamos si el marco de página [MODIFICAR COMENT]
                    if(S[j]->marcoDePagina == -1){
                        cout << "Page fault\n";
                        swapping(j);
                        procesos[i].cantPageFaults++;
                        cout << procesos[i].cantPageFaults << endl;
                    }

                    int dirReal = direccion % tamPagina + S[j]->marcoDePagina * tamPagina;
                    
                    //Ajustar cambios en memoria conforme a LRU
                    if(politica == "LRU"){
                        M[S[j]->marcoDePagina]->timestamp = tiempo;
                    }

                    cout << "Direccion Virtual es = " << direccion << " y direccion real = " << dirReal << endl;
                    
                    return;
                }
            }
        }
    } 
}

/*
    Funcion utilizada para liberar el proceso de memoria
    Parámetros:
        -Proceso: Representa el ID del proceso a liberar
*/
void liberarProceso(int proceso) {
    //Utilizado para registrar los marcos de pag. a liberar
    vector <int> marcosDePagina;

    //Utilizado para registrar las pag. a liberar
    vector <int> paginas;

    //Desplegar marcos de pagina ocupaos por el proceso
    cout << "Liberar los marcos de página ocupados por el proceso " << proceso << endl;
    
    //Ciclo para liberar los marcos de página ocupados por el proceso en la memoria real
    for(int i = 0; i < 128; i++){
        if(M[i] != NULL && proceso == (M[i]->idProceso)){
            marcosDePagina.push_back(i);
            tiempo += 0.1;
            delete M[i];
            M[i] = NULL;
            
        }
    }
 
    //Ciclo para liberar las páginas ocupadas por el proceso en la memoria virtual     
    for(int i = 0; i < 256; i++){
        if(S[i] != NULL && proceso == S[i]->idProceso){
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
    if(marcosDePagina.size() > 0){
        cout << "Se liberan los marcos de memoria real: [";
        for (int i = 0; i < marcosDePagina.size(); i++) {
                cout << marcosDePagina[i] << ",";
        }
        cout << "]" << endl;
    }
    else{
        cout << "El proceso no está ocupado en ningún marco de página\n";
    }
    if(paginas.size() > 0){
        cout << "Se liberan los marcos del área de swapping: [";
        for (int i = 0; i < paginas.size(); i++) {
            cout << paginas[i] << ",";
        }
        cout << "]" << endl;
    }
    else{
        cout << "El proceso no está ocupando ninguna página\n";
    }
}

/* Funcion utilizada */
void finCiclo() {

    if (procesos.size() > 0) {
        int turnaroundProm = 0;

        //El tiempo final es de -1
        for (int i = 0; i < procesos.size(); i++) {
            if (procesos[i].tiempoFinal == -1) {
                procesos[i].tiempoFinal = tiempo;
            }
            cout << "Turnaround del proceso " << procesos[i].idProceso << " es = " << procesos[i].tiempoFinal - procesos[i].tiempoInicio << endl;
            turnaroundProm += (procesos[i].tiempoFinal - procesos[i].tiempoInicio);
        }

        turnaroundProm /= procesos.size();
        cout << "Turnaround promedio es = " << turnaroundProm << endl;

        for (int i = 0; i < procesos.size(); i++) {
            cout << "La cantidad de Page Faults del proceso " << procesos[i].idProceso << " es = " << procesos[i].cantPageFaults << endl;
        }

        cout << "La cantidad de swaps es = " << cantSwaps << endl;
    }
    else {
        cout << "ERROR: No se ha cargado a memoria ningun proceso\n";
    }
}

/*
    Funcion utilizada para procesar la entrada, y saber que instrucción se quiere ejecutar
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

    cout << linea << endl; 

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
        /*getline(ss, extra);
        cout << extra << endl;*/
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
        cout << "ERROR: Instrucción invalida\n";
        break;
    }
    
    cout << endl;

    return valor;
}

/*Funcion principal para la lectura del archivo y reinicio del proceso*/
int main() {

    ifstream entrada;
    string linea, nombreArch = "PDF.txt";
    bool seguir;

    valoresIniciales();

    entrada.open(nombreArch);

    if (entrada.is_open()) {
        politica = "LRU";
        do {
            getline(entrada, linea);
            seguir = parsearInput(linea);
        } while (seguir);
    }

    entrada.close();

    reiniciarValores();

    entrada.open(nombreArch);

    if (entrada.is_open()) {
        politica = "FIFO";
        do {
            getline(entrada, linea);
            seguir = parsearInput(linea);
        } while (seguir);
    }

    entrada.close();
    
    return 0;
}
