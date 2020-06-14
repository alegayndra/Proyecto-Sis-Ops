/*
    Programa: Simulador de LRU y FIFO
    
    Descripción:
    Simula las politicas de reemplazo de LRU y FIFO utilizando dos vectores, uno que representa la memoria virtual (S) y otro la real (M).
    S es de 4096 bytes y M de 2048 bytes. El tamaño de página es de 16, por lo que hicimos los vectores M y S de 128 y 256 de tamaño respectivamente.

    Autores del programa:
    - Alberto García Viegas | A00822649
    - José Carlos | A00822554
    - Paola
    - Tanya
*/

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

// Vector de procesos
vector<Proceso> procesos;

// Variables globales
double tiempo;                  // El tiempo que ha transcurrido desde que inicio la simulación
int tamPagina;                  // El tamaño de página de las memorias
int cantSwaps;                  // La cantidad total de swaps que han ocurrido en la simulación
int cantPaginasVirtLibres;      // Cantidad de páginas libres en la memoria virtual
string politica;

/* Funcion donde se inicializa como NULL los vectores (ya que son de apuntadores) */
void valoresIniciales() {

    // Se inicializan las memorias con valores nulos
    for (int i = 0; i < 128; i++) {
        M.push_back(NULL);
        S.push_back(NULL);
        S.push_back(NULL);
    }

    // Se inicializan las variables globales con sus valores
    cantPaginasVirtLibres = S.size();
    tiempo = 0;
    tamPagina = 16;
    cantSwaps = 0;
}

/* Función utilizada para reiniciar los valores en la función de "fin" */
void reiniciarValores() {
    
    // Se limpian los vectores
    M.clear();
    S.clear();
    procesos.clear();

    // Se vuelven a inicializar las variables
    valoresIniciales();
}

/*
    Función para separar una lista de enteros en rangos y pasarlos a texto
    Parámetros:
        - vec: vector de enteros
    Output:
        - string: los rangos como texto
*/
string mostrarRangos(vector<int> &vec) {
    string extra = ""; // Valor que se va a regresar

    for (int i = 0; i < vec.size(); i++) {
        // Checa si hay valores consecutivos
        if (vec[i] == vec[i + 1] - 1) {

            // inicio y fin del rango
            int inicio, fin;
            inicio = vec[i];
            fin = inicio;
            i++;

            // Se cicla mientras haya números repetidos y no se llegue al fin del vector
            while (i < vec.size() && fin + 1 == vec[i]) {
                fin++;
                i++;
            }

            // Se concatena el rango al texto
            // e.g. si inicio = 1 y fin = 5, entonces se concatena 1-5
            extra += to_string(inicio) + "-" + to_string(fin);
        }
        else {
            extra += to_string(vec[i]);
        }

        // Checa si tiene que poner coma o ' y ' para separar los rangos
        if (i == vec.size() - 2) {
            extra += " y ";
        }
        else if (i < vec.size() - 1) {
            extra += ", ";
        }
    }

    return extra;
}

/*
    Función para hacer el reemplazo de páginas
    Parámetros:
    - posicion: posicion en S de la página de un proceso a la cual se le quiere asignar algún marco de página
*/
void swapping(int posicion) {

    cantSwaps++;
    tiempo += 1;

    double valor = INT_MAX; // Se va a guardar la timestamp menor de las paginas
    int posReal;            // Posicion en el vector de la memoria real

    // Se busca el valor menor de timestamp
    for (int i = 0; i < 128; i++) {
        if (M[i] != NULL && valor > M[i]->timestamp) {
            valor = M[i]->timestamp;
            posReal = i;
        }
    }

    // Se busca qué pagina de qué proceso tiene el marco de página que queremos utilizar
    for (int i = 0; i < 256; i++) {
        if (S[i] != NULL && S[i]->marcoDePagina == posReal) {

            // Esto es para marcar que no está en memoria real
            S[i]->marcoDePagina = -1; 

            // Se asigna el marco de página
            S[posicion]->marcoDePagina = posReal; 

            // Se actualiza la información del marco de página
            M[posReal]->idProceso = S[posicion]->idProceso;
            M[posReal]->timestamp = tiempo;
            M[posReal]->cantBytes = S[posicion]->cantBytes;

            cout << "La pagina " << S[posicion]->pagina << " del proceso " << S[posicion]->idProceso << " fue swappeada al marco " << S[posicion]->marcoDePagina << endl;
        }
    }
}

/*
    Función utilizada para cargar un procesos a memoria
    Parámetros:
        - bytes: representa el tamaño del proceso
        - proceso: representa el ID del proceso
 */
void cargarAMemoria(int bytes, int proceso) {

    // Checa si el proceso a cargar cabe en la memoria real
    if (bytes <= 2048) {

        int cantPaginas = ceil(bytes / tamPagina); // La cantidad de página que va a ocupar el nuevo proceso

        // Checa si el proceso a cargar cabe en la memoria virtual
        if (cantPaginasVirtLibres >= cantPaginas) {
            Proceso proc;               // Un proceso nuevo
            bool paginaEncontrada;      // Para determinar si ya se encontró un marco de página en memoria virtual
            int bytesExtra = bytes;     // Variable con el tamaño del programa

            // Agregamos n cantidad de paginas a la memoria virtual del proceso que se está cargando
            for (int i = 0; i < cantPaginas; i++) {
                paginaEncontrada = false;

                // Buscamos un marco de página vacío en la memoria virtual
                for (int j = 0; !paginaEncontrada && j < 256; j++) {

                    // Checa si el marco de página actual está vacío
                    if (S[j] == NULL) {

                        // Crea una nueva página en la memoria virtual
                        S[j] = new ProcesoVirtual;
                        S[j]->idProceso = proceso;
                        S[j]->pagina = i;

                        tiempo += 1;

                        cantPaginasVirtLibres--;

                        paginaEncontrada = true;

                        // Se busca un marco de página en la memoria real que esté vacío
                        bool memoriaEncontrada = false; // Determina
                        for (int j = 0; !memoriaEncontrada && j < M.size(); j++) {

                            // Checa si el marco de página actual está vacío
                            if (M[j] == NULL) {

                                // Se asigna la página al marco de página
                                M[j] = new ProcesoReal;
                                M[j]->idProceso = proceso;
                                M[j]->timestamp = tiempo;
                                M[j]->cantBytes = (bytesExtra > tamPagina) ? tamPagina : bytesExtra;

                                S[i]->cantBytes = (bytesExtra > tamPagina) ? tamPagina : bytesExtra;
                                S[i]->marcoDePagina = j;

                                bytesExtra -= tamPagina;

                                memoriaEncontrada = true;
                            }
                        }

                        // En caso de que no haya encontrado un espacio en memoria, se hace el reemplazo de páginas
                        if (!memoriaEncontrada) {
                            swapping(j);
                        }
                    }
                }
            }

            // Crea registro del proceso
            proc.idProceso = proceso;
            proc.cantPaginas = cantPaginas;
            proc.tiempoInicio = tiempo;
            proc.tamProceso = bytes;
            proc.tiempoFinal = -1; // Marca que no ha terminado
            proc.cantPageFaults = 0;

            procesos.push_back(proc);

            vector<int> marcos; // Para guardar los números de los marcos de página utilizados por el proceso cargado

            // Se buscan los marcos de página en la memoria real
            for (int i = 0; i < 128; i++) {
                if (M[i] != NULL && proceso == M[i]->idProceso) {
                    marcos.push_back(i);
                }
            }

            cout << "Se asignaron los marcos de pagina [" << mostrarRangos(marcos) << "]" << endl;
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
        - direccion: Direccion virtual
        - proceso:   ID del proceso
        - modificar: condicion para saber si fue modificado
*/
void accederADireccion(int direccion, int proceso, bool modificar) {
    cout << "Obtener la direccion real correspondiente a la direccion virtual " << direccion << " del proceso " << proceso << endl;
    tiempo += 0.1;

    // Calcular pagina
    int pagina = (direccion / tamPagina) - ((direccion % tamPagina == 0 && direccion != 0) ? 1 : 0);

    // Recorrer las paginas hasta que encontremos la pagina donde esta el proceso
    for(int j = 0; j < 256; j++){
        if(S[j] != NULL && S[j]->idProceso == proceso && S[j]->pagina == pagina){

            // Checamos si la página está cargada a memoria, si no hacemos swapping
            if(S[j]->marcoDePagina == -1){
                swapping(j);
                        
                // Busca el proceso en la lista de procesos para poder aumentar su cantidad de fallos de página
                for (int i = 0; i < procesos.size(); i++) {
                    if (proceso == procesos[i].idProceso) {
                        procesos[i].cantPageFaults++;
                    }
                }
            }

            // Calcula la dirección reals
            int dirReal = direccion % tamPagina + S[j]->marcoDePagina * tamPagina + ((direccion % tamPagina == 0 && direccion != 0) ? tamPagina : 0);
                    
            //Ajustar timestamp en memoria conforme a LRU
            if(politica == "LRU"){
                M[S[j]->marcoDePagina]->timestamp = tiempo;
            }

            cout << "Direccion Virtual es = " << direccion << " y Direccion Real = " << dirReal << endl;
               
            return;
        }
    }

    cout << "ERROR: No se encontro la direccion de memoria dada\n";
}

/*
    Funcion utilizada para liberar el proceso de memoria
    Parámetros:
        - Proceso: Representa el ID del proceso a liberar
*/
void liberarProceso(int proceso) {
    // Utilizado para registrar los marcos de pag. a liberar
    vector <int> marcosDePagina;

    // Utilizado para registrar las pag. a liberar
    vector <int> paginas;

    // Desplegar marcos de pagina ocupaos por el proceso
    cout << "Liberar los marcos de pagina ocupados por el proceso " << proceso << endl;
    
    // Ciclo para liberar los marcos de página ocupados por el proceso en la memoria real
    for(int i = 0; i < 128; i++){
        if(M[i] != NULL && proceso == (M[i]->idProceso)){
            marcosDePagina.push_back(i);
            tiempo += 0.1;
            delete M[i];
            M[i] = NULL;
            
        }
    }
 
    // Ciclo para liberar las páginas ocupadas por el proceso en la memoria virtual     
    for(int i = 0; i < 256; i++){
        if(S[i] != NULL && proceso == S[i]->idProceso){
            paginas.push_back(i);
            tiempo += 0.1;
            delete S[i];
            S[i] = NULL;
            cantPaginasVirtLibres++;
        }
    }

    // Ciclo para buscar el proceso que se acaba de liberar y marcar que ya terminó  
    for(int i = 0; i < procesos.size(); i++){
        if(proceso == procesos[i].idProceso){
            procesos[i].tiempoFinal = tiempo;
        }
    }

    // Checa si se liberaron marcos de página de la memoria real
    if(marcosDePagina.size() > 0){
        cout << "Se liberan los marcos de memoria real: [" << mostrarRangos(marcosDePagina) << "]" << endl;
    }
    else{
        cout << "El proceso no esta ocupando ningun marco de pagina\n";
    }

    // Checa si se liberaron marcos de página de la memoria virtual
    if(paginas.size() > 0){
        cout << "Se liberan los marcos del area de swapping: [" << mostrarRangos(paginas) << "]" << endl;
    }
    else{
        cout << "El proceso no esta ocupando ninguna pagina\n";
    }
}

/* Funcion utilizada */
void finCiclo() {

    // Checa si hay procesos cargados
    if (procesos.size() > 0) {
        double turnaroundProm = 0; // Acumulador para poder calcular el turnaround promedio

        cout << "--- Estadisticas de " << politica << " ---\n\n";
        cout << "Turnarounds individuales:\n";

        // Calcula el turnaround de todos los procesos
        for (int i = 0; i < procesos.size(); i++) {

            // Checamos si ya se terminó el proceso
            // Asumimos que todos los procesos que no han terminado no hay necesidad de liberarlos y que terminan exactamente en este momento
            if (procesos[i].tiempoFinal == -1) {
                procesos[i].tiempoFinal = tiempo;
            }

            cout << "- Proceso " << procesos[i].idProceso << ": " << procesos[i].tiempoFinal - procesos[i].tiempoInicio << endl;
            
            turnaroundProm += (procesos[i].tiempoFinal - procesos[i].tiempoInicio);
        }

        turnaroundProm /= double(procesos.size());
        cout << "\nTurnaround promedio: " << turnaroundProm << endl;

        cout << "\nPage faults:\n";

        for (int i = 0; i < procesos.size(); i++) {
            cout << "- Proceso " << procesos[i].idProceso << ": " << procesos[i].cantPageFaults << endl;
        }

        cout << "\nCantidad de swaps: " << cantSwaps << endl;
    }
    else {
        cout << "ERROR: No se ha cargado a memoria ningun proceso\n";
    }
}

/*
    Funcion utilizada para procesar la entrada y saber que instrucción se quiere ejecutar
    Parámetros:
        - línea: La instrucción a procesar
    Output:
        - bool: Especifíca si ya se terminó el proceso
*/
bool parsearInput(string linea) {
    stringstream ss;    // Stringstream para parsear el input
    string n, p, d, m;  // Variables auxiliares para sacar los valores de la instrucción
    char c;             // Caracter que tendrá la instrucción a ejecutar
    bool valor = true;  // Para saber si seguir el programa o no

    // Carga la instrucción al stringstream y saca la instrucción a ejecutar
    ss << linea;
    ss >> c;

    switch (c) {
    case 'P':
        ss >> n >> p;
        cout << "P " << n << " " << p << endl;
        cargarAMemoria(stoi(n), stoi(p));
        break;
    case 'A':
        ss >> d >> p >> m;
        cout << "A " << d << " " << p << " " << m <<endl;
        accederADireccion(stoi(d), stoi(p), stoi(m));
        break;
    case 'L':
        ss >> p;
        cout << "L " << p << endl;
        liberarProceso(stoi(p));
        break;
    case 'C':
        cout << linea << endl;
        break;
    case 'F':
        cout << "F\n";
        finCiclo();
        reiniciarValores();
        break;
    case 'E':
        // Suena dubstep en el fondo
        cout << "E\n";
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

/* Funcion principal para la lectura del archivo y reinicio del proceso */
int main() {

    ifstream entrada;           // Archivo de entrada
    string linea, nombreArch;   // Nombre del archivo de entrada
    bool seguir;                // Determina si se sigue ejecutando el programa


    // Se pide el archivo de entrada
    cout << "Nombre archivo\n";
    cin >> nombreArch;

    // Inicializa las variables
    valoresIniciales();

    entrada.open(nombreArch);

    // Empieza la simulación para LRU
    if (entrada.is_open()) {
        cout << "\n--- LRU ---\n\n";
        politica = "LRU";
        do {
            getline(entrada, linea);
            seguir = parsearInput(linea);
        } while (seguir);
    }

    entrada.close();

    // Reinicia los valores para poder volver a ejecutar la simulación
    reiniciarValores();

    entrada.open(nombreArch);

    // Empieza la simulación para LRU
    if (entrada.is_open()) {
        cout << "\n--- FIFO ---\n\n";
        politica = "FIFO";
        do {
            getline(entrada, linea);
            seguir = parsearInput(linea);
        } while (seguir);
    }

    entrada.close();
    
    return 0;
}
