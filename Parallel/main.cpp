// Autor: Yair Antonio Castillo Castillo

//Diseñar y codificar una paralelización a los algoritmos de suma/resta, multiplicación e inversa (por medio de reducción Gauss-Jordan) de matrices. Así mismo, realizar un reporte donde se presente una comparativa de los resultados de los algoritmos secuenciales y paralelos probados con matrices de diferentes tamaños. Explicar el funcionamiento de los algoritmos codificados.
#include <pthread.h>
#include <iostream>
#include "matriz.hpp"
#include <chrono>

#define HAVE_STRUCT_TIMESPEC
#define NTHREADS 4
using namespace std;

struct Argumentos{  // Aquí definimos los argumentos que tomará las funciones
    int ini;        // Límites
    int fin;
    int pivote;     // Para la inversa
};

int n1 = 0;  // Defimos los tamaños de las matrices, con 0 ya que después podemos pedir
int m1 = 0;  // a la persona que inserte las dimensiones que desee
int n2 = 0;
int m2 = 0;

matriz A(n1, m1); // Defimos las matrices principales
matriz B(n2, m2);

matriz CS(n1, m1); // Defimos las matrices de los resultados
matriz CR(n1, m1);
matriz CM(n1, m2);
matriz CI(n1, m1);

void* suma_paral(void *args){                           // Función de suma en paralelo
    Argumentos *_args = (Argumentos*) args;             // Definimos las variables
    for (int i = ((_args->ini)-1); i <= ((_args->fin)-1); i++){// Aquí sumamos el fina inicial y la final
        for (int j = 0; j < B.ncolumnas; j++){          // que viene en argumentos para cada hilo
            CS.Tab[i][j] = A.Tab[i][j] + B.Tab[i][j];   // Sumamos cada elemento de las filas
        }
    }
    return NULL;
}

void* resta_paral(void *args){                        // Función de resta en paralelo
    Argumentos *_args = (Argumentos*) args;
    for (int i = ((_args->ini)-1); i <= ((_args->fin)-1); i++){// Aquí sumamos el fina inicial y la final
        for (int j = 0; j < B.ncolumnas; j++){
            CR.Tab[i][j] = A.Tab[i][j] - B.Tab[i][j]; // Restamos cada elemento de las filas
        }
    }
    return NULL;
}

void* mult_paral(void *args){                       // Función de multiplicación en paralelo
    Argumentos *_args = (Argumentos*) args;         // Argumentos
    for (int i = ((_args->ini)-1); i <= ((_args->fin)-1); i++){// Aquí sumamos el fina inicial y la final
        for (int j = 0; j < m2; j++){        // En cada entrada se multiplica el renglón de la 1º matriz por la columna de la 2º matriz
            double y = 0;                        // Aqui guardamos la suma de la multiplicación
            for (int k = 0; k < m1; k++){
                y=y+A.Tab[i][k]*B.Tab[k][j];
            }
            CM.Tab[i][j] += y;                // Aqui se asigna a cada entrada la suma
        }
    }
    return NULL;
}

void inter_filas(int pivot){
    int i=pivot;                     // Aqui se empieza haciendo la matriz escalonado por renglones, donde se ira eliminando todos
    double a = A.Tab[i][i];          // los elementos que estén debajo de la diagonal
    int k=0;
    while (a==0){                    // Si el elemento de la diagonal es 0, ver que exista un elemento abajo de esa diagonal que no sea 0
        for (k=i;k<A.nfilas;k++){
            a = A.Tab[k][i];         // Ver cada elemento de la columna
            if (a!=0){
                break;
            }else if (k==(A.nfilas) && a==0){// En caso no es existir, entonces regresar error de que no existe inversa para esa matriz
                cout << "La matriz no es invertible " << endl;
                cout << endl;
                A.Tab = NULL;       // Aqui se regresa un apuntador doble igua a NULL
                return;
            }
        }
    }
    if (k>i){                       // Si la diagonal fue 0 y existe otro valor abajo que no es 0, entonces se intercambian los renglones
        matriz aux(1,A.nfilas);
        matriz aux1(1,A.nfilas);
        aux.Tab[1] = A.Tab[k];      // Se intercambian para la matriz original
        A.Tab[k]=A.Tab[i];
        A.Tab[i] = aux.Tab[1];
        aux1.Tab[1] = CI.Tab[k];    // Y tambien se cambia en la matriz identidad (Ya modificada si no es la primera interacion)
        CI.Tab[k]=CI.Tab[i];
        CI.Tab[i] = aux1.Tab[1];
    }
}

void* inversa_paral(void *args){            // Funcion para obtener la inversa usando Gauss-Jordan
    Argumentos *_args = (Argumentos*) args; // Los argumentos
    int filacom = (_args->ini)-1;           // Definimos variables
    int filafin = (_args->fin)-1 ;
    int i = (_args->pivote);
    double a = A.Tab[i][i];
    for (int k=0;k<A.nfilas;k++){           // Empezamos diviendo la fila entre el pivote
        CI.Tab[i][k]=CI.Tab[i][k]/a;
        A.Tab[i][k]=A.Tab[i][k]/a;
    }
    a = A.Tab[i][i]=1;                      // Hacemos el pivote
    for (int p=filacom;p<=filafin;p++){     // Empezamos a eliminar las filas que le dimos a cada hilo
        if (p!=i){                          // Aquí por si a un hilo le toca la fila que toma como pivote que no haga nada
            double b = A.Tab[p][i];         // Empiezan a eliminar la filas
            for (int j=0;j<A.ncolumnas;j++){
                A.Tab[p][j] = (double) A.Tab[p][j] - (double)A.Tab[i][j]*(double)b;
                CI.Tab[p][j] = (double)CI.Tab[p][j]-(double)CI.Tab[i][j]*(double)b;
            }
        }
    }
    return NULL;
}

int main(){
    int opcion;
    string imprimir;
    do{                                      // Las operaciones de las matrices
        cout << "Elige la opcion de la operacion de algebra de matrices que se desea hacer: " << endl;
        cout << "1. Sumar 2 matrices" << endl;
        cout << "2. Restar 2 matrices" << endl;
        cout << "3. Multiplicar 2 matrices" << endl;
        cout << "4. Obterner la inversa" << endl;
        cout << "5. Todas las anteriores juntas" << endl;
        cout << "6. Salir" << endl;
        std::cout << "Opción:";
        std::cin >> opcion;
        switch(opcion){
            case 1:{    // La primer operación es la suma, lo cual pedirá que inserte las dimensiones de las 2 matrices
                cout << "Inserte el numero de renglones de la 1º matriz: ";
                cin >> n1;
                cout << "Inserte el numero de columnas de la 1º matriz: ";
                cin >> m1;
                cout << "Inserte el numero de renglones de la 2º matriz: ";
                cin >> n2;
                cout << "Inserte el numero de columnas de la 2º matriz: ";
                cin >> m2;
                A = matriz(n1, m1); // Defimos las matrices principales
                B = matriz(n2, m2);
                CS = matriz(n1, m1); // Defimos las matrices de los resultados
                matriz CSP(n1,m1);
                A.generacion_aleatoria();  // Llevamos de número
                B.generacion_aleatoria();
                pthread_t *thr = new pthread_t[NTHREADS]; //inicializo un array con los threads
                Argumentos *args = new Argumentos[NTHREADS];   // inicializo un array con los argumentos para cada thread
                int subint = floor(n1/NTHREADS);
                cout << endl;
                cout << "¿Desea imprimir las matrices? (y/n): ";  // Ver si quieren que imprima la matriz
                cin >> imprimir;
                if (imprimir=="y"){   // Si la respuesta es sí, se imprime A y B
                    cout << "La primer matriz es:" << endl;
                    A.print();
                    cout << endl;
                    cout << "La segunda matriz es:" << endl;
                    B.print();
                    cout << endl;
                }
                if (A.nfilas!=B.nfilas || A.ncolumnas!=B.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
                    cout << "Las dimensiones no son iguales por lo tanto no se pueden sumar" << endl; // Aqui se regresa un mensaje de error
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){ // Solo está definiendo los rangos de donde a donde va a sumar
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr; // Definimos la asignación de tareas a los hilos
                        pthread_attr_init(&attr); // Incializamos
                        pthread_create(&thr[i],&attr, suma_paral, &args[i]); // Aquí creamos los hilos
                    }
                    for(int i=0;i<NTHREADS;i++){ // A que terminen todos los hilos
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                    cout << "La suma paralela es" << endl;
                    if (imprimir=="y"){   // Igual para secuencial
                        CS.print();
                    }
                    std::cout<<"Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
                CSP = A + B;              // Se usa el operador "+"
                std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CSP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count())/1000000.0<<std::endl;
                cout << endl;
                delete[] thr;
                delete[] args;
                break;
            }
            case 2:{  // La segunda operación es la resta, lo cual pedirá que inserte las dimensiones de las 2 matrices
                cout << "Se restara la 1º matriz menos la 2º matriz (en ese orden) " << endl;
                cout << "Inserte el numero de renglones de la 1º matriz: ";
                cin >> n1;
                cout << "Inserte el numero de columnas de la 1º matriz: ";
                cin >> m1;
                cout << "Inserte el numero de renglones de la 2º matriz: ";
                cin >> n2;
                cout << "Inserte el numero de columnas de la 2º matriz: ";
                cin >> m2;
                A = matriz(n1, m1); // Defimos las matrices principales
                B = matriz(n2, m2);
                CR = matriz(n1, m1); // Defimos las matrices de los resultados
                matriz CRP(n1,m1);
                A.generacion_aleatoria();
                B.generacion_aleatoria();
                pthread_t *thr = new pthread_t[NTHREADS]; //inicializo un array con los threads
                Argumentos *args = new Argumentos[NTHREADS];   // inicializo un array con los argumentos para cada thread
                int subint = floor(n1/NTHREADS);
                cout << endl;
                cout << "¿Desea imprimir las matrices? (y/n): "; // Ver si quieren que imprima la matriz
                cin >> imprimir;
                if (imprimir=="y"){  // Si la respuesta es sí, se imprime A y B
                    cout << "La primer matriz es:" << endl;
                    A.print();
                    cout << endl;
                    cout << "La segunda matriz es:" << endl;
                    B.print();
                    cout << endl;
                }
                if (A.nfilas!=B.nfilas || A.ncolumnas!=B.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
                    cout << "Las dimensiones no son iguales por lo tanto no se pueden restar" << endl; // Aqui se regresa un mensaje de error
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){ // Solo está definiendo los rangos de donde a donde va a sumar
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr;   // Definimos la asignación de tareas a los hilos
                        pthread_attr_init(&attr);  // Incializamos
                        pthread_create(&thr[i],&attr, resta_paral, &args[i]); // Aquí creamos los hilos
                    }
                    for(int i=0;i<NTHREADS;i++){ // A que terminen todos los hilos
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
                    cout << "La resta paralela es" << endl;  // Igual para secuencial
                    if (imprimir=="y"){
                        CR.print();
                    }
                    std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
                CRP = A - B;              // Se usa el operador "-"
                std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CRP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end3-begin3).count())/1000000.0<<std::endl;
                cout << endl;
                delete[] thr;
                delete[] args;
                break;
            }
            case 3:{   // La tercera operación es la multiplicación, lo cual pedirá que inserte las dimensiones de las 2 matrices
                cout << "Se multiplicará la 1º matriz por la 2º matriz (en ese orden)" << endl;
                cout << "Para esta operacion tiene que coincidir el no. de columnas de la 1º matriz con el no. de renglones de la 2º matriz" << endl;
                cout << endl;
                std::cout << "Inserte el numero de renglones de la 1º matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la 1º matriz: ";
                std::cin >> m1;
                std::cout << "Inserte el numero de renglones de la 2º matriz: ";
                std::cin >> n2;
                std::cout << "Inserte el numero de columnas de la 2º matriz: ";
                std::cin >> m2;
                cout << endl;
                A = matriz(n1, m1); // Defimos las matrices principales
                B = matriz(n2, m2);
                CM = matriz(n1, m2); // Defimos las matrices de los resultados
                matriz CMP(n1,m2);
                A.generacion_aleatoria();
                B.generacion_aleatoria();
                pthread_t *thr = new pthread_t[NTHREADS]; //inicializo un array con los threads
                Argumentos *args = new Argumentos[NTHREADS];   // inicializo un array con los argumentos para cada thread
                int subint = floor(n1/NTHREADS);
                cout << "¿Desea imprimir las matrices? (y/n): "; // Ver si quieren que imprima la matriz
                cin >> imprimir;
                if (imprimir=="y"){
                    cout << "La primer matriz es:" << endl;
                    A.print();
                    cout << endl;
                    cout << "La segunda matriz es:" << endl;
                    B.print();
                    cout << endl;
                }
                if (A.ncolumnas!=B.nfilas){      // Aqui se checa si el # de columnas de la 1º matriz coincide con el # de renglones de la 2º matriz
                    cout << "El numero de columnas de la 1º matriz no coicididen con el numero de renglonoes de la 2º matriz, por lo tanto no se pueden multiplicar" << endl;
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){ // Solo está definiendo los rangos de donde a donde va a sumar
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr;
                        pthread_attr_init(&attr);
                        pthread_create(&thr[i],&attr, mult_paral, &args[i]);
                    }
                    for(int i=0;i<NTHREADS;i++){
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();
                    cout << "La multiplicación paralela es" << endl;
                    if (imprimir=="y"){
                        CM.print();
                    }
                    std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end4-begin4).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin5 = std::chrono::steady_clock::now();
                CMP = A * B;              // Se usa el operador "*"
                std::chrono::steady_clock::time_point end5 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CMP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end5-begin5).count())/1000000.0<<std::endl;
                cout << endl;
                delete[] thr;
                delete[] args;
                break;
            }
            case 4:{
                std::cout << "Inserte el numero de renglones de la matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la matriz: ";
                std::cin >> m1;
                cout << endl;
                pthread_t *thr = new pthread_t[NTHREADS]; //inicializo un array con los threads
                Argumentos *args = new Argumentos[NTHREADS];   // inicializo un array con los argumentos para cada thread
                int subint = floor(n1/NTHREADS);
                A = matriz(n1, m1); // Definimos las matriz principal
                CI = matriz(n1, m1); // Definimos las matriz de resultado
                matriz CIP(n1,m1);
                matriz A2(n1,m1);
                A.generacion_aleatoria();
                cout << endl;
                cout << "¿Desea imprimir las matrices? (y/n): "; // Ver si quieren que imprima la matriz
                cin >> imprimir;
                if (imprimir=="y"){
                    cout << "La matriz es:" << endl;
                    A.print();
                    cout << endl;
                }
                for (int i=0;i<n1;i++){ // Duplicamos A a A2
                    for (int j=0;j<m1;j++){
                        A2.Tab[i][j]=A.Tab[i][j];
                    }
                }
                if (A.ncolumnas!=A.nfilas){  // Aqui se checa si la matriz es cuadrada para ver si podemos obtener la inversa
                    cout << "No se puede obtener la inversa, la matriz tiene que ser cuadrada" << endl; // Aqui se regresa un mensaje de error
                    cout << endl;
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin6 = std::chrono::steady_clock::now();
                    for (int i=0; i<CI.nfilas;i++){            // (Creeación de inversa) Agregamos ceros a la afuera de la diagonal
                        for(int j=0;j<CI.ncolumnas;j++){       // Y 1 en la diagonal
                            CI.Tab[i][j] = (double) 0;         // 0 en todas las entradas
                            if (i==j){
                                CI.Tab[i][j] =  (double) 1;    // 1 en la diagonal
                            }
                        }
                    }
                    for (int j=0;j<CI.ncolumnas;j++){  // Aquí hacemos para cada pivote, ya que esta parte se secuencial
                        inter_filas(j);
                        if (A.Tab==NULL){
                            break;
                        }
                        for (int i=0; i<NTHREADS; i++){ // Solo está definiendo los rangos de donde a donde va a sumar
                            if(i==NTHREADS-1){
                                args[i].ini = subint*i+1;
                                args[i].fin = n1;
                                args[i].pivote=j;
                            }else{
                                args[i].ini = subint*i+1;
                                args[i].fin = subint*(i+1);
                                args[i].pivote=j;
                            }
                            pthread_attr_t attr;
                            pthread_attr_init(&attr);
                            pthread_create(&thr[i],&attr, inversa_paral, &args[i]);
                        }
                        for (int i=0;i<NTHREADS;i++){
                            pthread_join(thr[i], NULL);
                        }
                    }
                    std::chrono::steady_clock::time_point end6 = std::chrono::steady_clock::now();
                    if (A.Tab!=NULL){
                        cout << "La inversa paralale es" << endl;
                        if (imprimir=="y"){
                            CI.print();
                        }
                        std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end6-begin6).count())/1000000.0<<std::endl;
                    }
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin7 = std::chrono::steady_clock::now();
                CIP = A2.inversa();
                std::chrono::steady_clock::time_point end7 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CIP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end7-begin7).count())/1000000.0<<std::endl;
                cout << endl;
                delete[] thr;
                delete[] args;
                break;
            }
            case 5:{    // La primer operación es la suma, lo cual pedirá que inserte las dimensiones de las 2 matrices
                cout << "Inserte el numero de renglones de la 1º matriz: ";
                cin >> n1;
                cout << "Inserte el numero de columnas de la 1º matriz: ";
                cin >> m1;
                cout << "Inserte el numero de renglones de la 2º matriz: ";
                cin >> n2;
                cout << "Inserte el numero de columnas de la 2º matriz: ";
                cin >> m2;
                pthread_t *thr = new pthread_t[NTHREADS]; //inicializo un array con los threads
                Argumentos *args = new Argumentos[NTHREADS];   // inicializo un array con los argumentos para cada thread
                int subint = floor(n1/NTHREADS);
                A = matriz(n1, m1); // Defimos las matrices principales
                B = matriz(n2, m2);
                CS = matriz(n1, m1); // Defimos las matrices de los resultados
                CR = matriz(n1, m1); // Defimos las matrices de los resultados
                CM = matriz(n1, m2); // Defimos las matrices de los resultados
                CI = matriz(n1, m1); // Defimos las matrices de los resultados
                matriz CMP(n1,m2);
                matriz CSP(n1,m1);
                matriz CRP(n1,m1);
                matriz CIP(n1,m1);
                matriz A2(n1,m1);
                A.generacion_aleatoria();
                B.generacion_aleatoria();
                cout << endl;
                cout << "¿Desea imprimir las matrices? (y/n): "; // Ver si quieren que imprima la matriz
                cin >> imprimir;
                if (imprimir=="y"){
                    cout << "La primer matriz es:" << endl;
                    A.print();
                    cout << endl;
                    cout << "La segunda matriz es:" << endl;
                    B.print();
                    cout << endl;
                }
                //SUMA
                if (A.nfilas!=B.nfilas || A.ncolumnas!=B.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
                    cout << "Las dimensiones no son iguales por lo tanto no se pueden sumar" << endl; // Aqui se regresa un mensaje de error
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){ // Solo está definiendo los rangos de donde a donde va a sumar
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr;
                        pthread_attr_init(&attr);
                        pthread_create(&thr[i],&attr, suma_paral, &args[i]);
                    }
                    for(int i=0;i<NTHREADS;i++){
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                    cout << "La suma paralela es" << endl;
                    if (imprimir=="y"){
                        CS.print();
                    }
                    std::cout<<"Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
                CSP = A + B;              // Se usa el operador "+"
                std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CSP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end1-begin1).count())/1000000.0<<std::endl;
                cout << endl;
                //RESTA
                if (A.nfilas!=B.nfilas || A.ncolumnas!=B.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
                    cout << "Las dimensiones no son iguales por lo tanto no se pueden restar" << endl; // Aqui se regresa un mensaje de error
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr;
                        pthread_attr_init(&attr);
                        pthread_create(&thr[i],&attr, resta_paral, &args[i]);
                    }
                    for(int i=0;i<NTHREADS;i++){
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
                    cout << "La resta paralela es" << endl;
                    if (imprimir=="y"){
                        CR.print();
                    }
                    std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end2-begin2).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
                CRP = A - B;              // Se usa el operador "-"
                std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CRP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end3-begin3).count())/1000000.0<<std::endl;
                cout << endl;
                //MULTIPLICACIÓN
                if (A.ncolumnas!=B.nfilas){      // Aqui se checa si el # de columnas de la 1º matriz coincide con el # de renglones de la 2º matriz
                    cout << "El numero de columnas de la 1º matriz no coicididen con el numero de renglonoes de la 2º matriz, por lo tanto no se pueden multiplicar" << endl;
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
                    for(int i=0; i<NTHREADS; i++){
                        if(i==NTHREADS-1){
                            args[i].ini = subint*i+1;
                            args[i].fin = n1;
                        }else{
                            args[i].ini = subint*i+1;
                            args[i].fin = subint*(i+1);
                        }
                        pthread_attr_t attr;
                        pthread_attr_init(&attr);
                        pthread_create(&thr[i],&attr, mult_paral, &args[i]);
                    }
                    for(int i=0;i<NTHREADS;i++){
                        pthread_join(thr[i], NULL);
                    }
                    std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();
                    cout << "La multiplicación paralela es" << endl;
                    if (imprimir=="y"){
                        CM.print();
                    }
                    std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end4-begin4).count())/1000000.0<<std::endl;
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin5 = std::chrono::steady_clock::now();
                CMP = A * B;              // Se usa el operador "*"
                std::chrono::steady_clock::time_point end5 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CMP.print();                // Se imprime la matriz de la resta de la matrices
                
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end5-begin5).count())/1000000.0<<std::endl;
                cout << endl;
                
                cout << endl;
                for (int i=0;i<n1;i++){
                    for (int j=0;j<m1;j++){
                        A2.Tab[i][j]=A.Tab[i][j];
                    }
                }
                if (A.ncolumnas!=A.nfilas){  // Aqui se checa si la matriz es cuadrada para ver si podemos obtener la inversa
                    cout << "No se puede obtener la inversa, la matriz tiene que ser cuadrada" << endl; // Aqui se regresa un mensaje de error
                    cout << endl;
                    break;
                }else{
                    std::chrono::steady_clock::time_point begin6 = std::chrono::steady_clock::now();
                    for (int i=0; i<CI.nfilas;i++){            // (Creeación de inversa) Agregamos ceros a la afuera de la diagonal
                        for(int j=0;j<CI.ncolumnas;j++){       // Y 1 en la diagonal
                            CI.Tab[i][j] = (double) 0;         // 0 en todas las entradas
                            if (i==j){
                                CI.Tab[i][j] =  (double) 1;    // 1 en la diagonal
                            }
                        }
                    }
                    for (int j=0;j<CI.ncolumnas;j++){
                        inter_filas(j);
                        if (A.Tab==NULL){
                            break;
                        }
                        for (int i=0; i<NTHREADS; i++){
                            if(i==NTHREADS-1){
                                args[i].ini = subint*i+1;
                                args[i].fin = n1;
                                args[i].pivote=j;
                            }else{
                                args[i].ini = subint*i+1;
                                args[i].fin = subint*(i+1);
                                args[i].pivote=j;
                            }
                            pthread_attr_t attr;
                            pthread_attr_init(&attr);
                            pthread_create(&thr[i],&attr, inversa_paral, &args[i]);
                        }
                        for (int i=0;i<NTHREADS;i++){
                            pthread_join(thr[i], NULL);
                        }
                    }
                    std::chrono::steady_clock::time_point end6 = std::chrono::steady_clock::now();
                    if (A.Tab!=NULL){
                        cout << "La inversa paralale es" << endl;
                        if (imprimir=="y"){
                            CI.print();
                        }
                        std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end6-begin6).count())/1000000.0<<std::endl;
                    }
                }
                cout << endl;
                std::chrono::steady_clock::time_point begin7 = std::chrono::steady_clock::now();
                CIP = A2.inversa();
                std::chrono::steady_clock::time_point end7 = std::chrono::steady_clock::now();
                if (imprimir=="y"){
                    CIP.print();                // Se imprime la matriz de la resta de la matrices
                }
                std::cout << "Tiempo(seg)= "<<(std::chrono::duration_cast<std::chrono::microseconds>(end7-begin7).count())/1000000.0<<std::endl;
                cout << endl;
                delete[] thr;
                delete[] args;
                break;
            }
        }
        cout << endl;
    }
    while(opcion<6);
    printf("Gracias por usar este programa\n");
    return 0;
}
