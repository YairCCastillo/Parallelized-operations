// Autor: Yair Antonio Castillo Castillo

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "matriz.hpp"

void matriz::operator=(const matriz M){      // Funcion para copiar matrices (Se usa sobrecarga de operadores)
    ncolumnas = M.ncolumnas;
    nfilas = M.nfilas;
    if (nfilas == 0 || ncolumnas == 0 ){     // Aqui se checa si cumplen que las dimensiones son 0
        this->Tab=NULL;                      // en caso de que las dimensiones no coincidan
        return;                              // Aqui se regresa un apuntador doble igua a NULL
    }else{
        this->Tab = new double *[nfilas];    // Aquí igualamos las matriz a una nueva
        for (int i = 0; i < nfilas; i++){
            this->Tab[i] = new double[ncolumnas];
        }
        for (int i = 0; i < nfilas; i++){    // Aquí igualamos cada entrada
            for (int j = 0; j < ncolumnas; j++){
                Tab[i][j] = M.Tab[i][j];
            }
        }
    }
}

matriz matriz::operator+(const matriz &M){   // Funcion para sumar matrices (Se usa sobrecarga de operador)
    matriz M1(nfilas,ncolumnas);             // Aqui se crea matriz extra donde se guardara el resultado de la suma
    if (nfilas!=M.nfilas || ncolumnas!=M.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
        cout << "Las dimensiones no son iguales por lo tanto no se pueden sumar" << endl; // Aqui se regresa un mensaje de error
        M1.Tab=NULL;                        // en caso de que las dimensiones no coincidan
        return M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<M.nfilas;i++){           // En cada entrada de la matriz creada se suma las entradas de las matrices
        for(int j=0;j<M.ncolumnas;j++){
            M1.Tab[i][j] = (this->Tab)[i][j]+M.Tab[i][j];
        }
    }
    cout << "La suma sequencial es:" << endl; // Aquí se regresa un mensaje
    return M1;                              // Aqui se regresa la matriz que es la suma de las matrices
}

matriz matriz::operator-(const matriz &M){   // Funcion para restar matrices (Se usa sobrecarga de operador)
    matriz M1(nfilas,ncolumnas);             // Aqui se crea matriz extra donde se guardara el resultado de la resta
    if (this->nfilas!=M.nfilas || this->ncolumnas!=M.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
        cout << "Las dimensiones no son iguales por lo tanto no se puede restar" << endl; // Aqui se regresa un mensaje de error
        M1.Tab=NULL;                        // en caso de que las dimensiones no coincidan
        return M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<M.nfilas;i++){           // En cada entrada de la matriz creada restamos las entradas de las matrices
        for(int j=0;j<M.ncolumnas;j++){
            M1.Tab[i][j] = Tab[i][j]-M.Tab[i][j];
        }
    }
    cout << "La resta sequencial es:" << endl; // Aquí se regresa un mensaje
    return M1;                              // Aquí se regresa la matriz que es la resta de las matrices
}

matriz matriz::operator*(const matriz &M){   // Funcion para multiplicar matrices (Se usa sobrecarga de operador)
    matriz M1(nfilas,M.ncolumnas);           // Aqui se crea matriz extra donde se guardara el resultado de la multiplicación
    if (this->ncolumnas!=M.nfilas){          // Aqui se checa si el # de columnas de la 1º matriz coincide con el # de renglones de la 2º matriz
        cout << "El numero de columnas de la 1º matriz no coicididen con el numero de renglonoes de la 2º matriz, por lo tanto no se pueden multiplicar" << endl;
        M1.Tab=NULL;                        //  Aqui se regresa un mensaje de error en caso de que las dimensiones no coincidan
        return M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<nfilas;i++){             // En cada entrada de la matriz creada se multiplica el renglón de la 1º matriz
        for(int j=0;j<M.ncolumnas;j++){      // por la columna de la 2º matriz
            int y=0;                         // Aqui guardamos la suma de la multiplicación
            for (int k=0;k<(this->ncolumnas);k++){
                y=y+Tab[i][k]*M.Tab[k][j];
            }
            M1.Tab[i][j] = y;               // Aqui se asigna a cada entrada la suma
        }
    }
    cout << "La multiplicacion sequencial es:" << endl;
    return M1;                              // Aqui se regresa la matriz que es la multiplicacion de las matrices
    
}
    
matriz matriz::inversa(){                   // Funcion para obtener la inversa usando Gauss-Jordan
    matriz Inver(nfilas,ncolumnas);    // Aqui se crea una matriz extra donde se guardara la inversa con las dimensiones cambiadas
    if (ncolumnas!=nfilas){                  // Aqui se checa si la matriz es cuadrada para ver si podemos obtener la inversa
        cout << "No se puede obtener la inversa, las matriz tiene que ser cuadrada" << endl; // Aqui se regresa un mensaje de error
        cout << endl;                        // en caso de que la matriz no es cuadrada
        Inver.Tab=NULL;                     // Aqui se regresa un apuntador doble igua a NULL
        return Inver;
    }
    for (int i=0; i<nfilas;i++){             // Para obterner la inversa se tiene que crear una matriz identidad cuadrada y poder aplicar
        for(int j=0;j<ncolumnas;j++){        // Gauss-Jordan
            Inver.Tab[i][j] = (double) 0;   // 0 en todas las entradas
            if (i==j){
                Inver.Tab[i][j] =  (double) 1; // 1 en la diagonal
            }
        }
    }
    for (int i=0;i<(nfilas);i++){            // Aqui se empieza haciendo la matriz escalonado por renglones, donde se ira eliminando todos
        double a = Tab[i][i];                // los elementos que estén debajo de la diagonal
        int k=0;
        while (a==0){                       // Si el elemento de la diagonal es 0, ver que exista un elemento abajo de esa diagonal que no sea 0
            for (k=i;k<nfilas;k++){
                a = Tab[k][i];      // Ver cada una
                if (a!=0){
                    break;
                }else if (k==(nfilas) && a==0){// En caso no es existir, entonces regresar error de que no existe inversa para esa matriz
                    cout << "La matriz no es invertible " << endl;
                    cout << endl;
                    Inver.Tab = NULL;       // Aqui se regresa un apuntador doble igua a NULL
                    return Inver;
                }
            }
        }
        if (k>i){                       // Si la diagonal fue 0 y existe otro valor abajo que no es 0, entonces se intercambian los renglones
            matriz aux(1,nfilas);
            matriz aux1(1,nfilas);
            aux.Tab[1] = Tab[k];      // Se intercambian para la matriz original
            Tab[k]=Tab[i];
            Tab[i] = aux.Tab[1];
            aux1.Tab[1] = Inver.Tab[k];    // Y tambien se cambia en la matriz identidad (Ya modificada si no es la primera interacion)
            Inver.Tab[k]=Inver.Tab[i];
            Inver.Tab[i] = aux1.Tab[1];
        }
        for (int k=0;k<nfilas;k++){  // Aquí dividimos la fila del pivote entre el pivote
            Inver.Tab[i][k]=Inver.Tab[i][k]/a;
            Tab[i][k]=Tab[i][k]/a;
        }
        a = Tab[i][i]=1;            // Aquí vamos eliminando de arriba para abajo para cada pivote
        for (int p=0;p<nfilas;p++){
            if (p!=i){
                double b = Tab[p][i];
                for (int j=0;j<ncolumnas;j++){
                    Tab[p][j] = (double) Tab[p][j] - (double)Tab[i][j]*(double)b;
                    Inver.Tab[p][j] = (double)Inver.Tab[p][j]-(double)Inver.Tab[i][j]*(double)b;
                }
            }
        }
    }
    cout << "La matriz inversa sequencial es:" << endl; // Aqui se imprime un mensaje
    return Inver;                           // Regresamos la matriz transpuesta
}

matriz::matriz(int filas, int columnas){     // Funcion para crear la matrices de forma dinámica (Constructor)
    nfilas = filas;                          // Aqui se define el numero de filas
    ncolumnas = columnas;                    // Aqui se define el numero de filas
    Tab = new double *[nfilas];              // Aqui se crea un apuntador de tamaño nfilas
    for (int i=0;i<nfilas;i++){
        Tab[i]=new double[ncolumnas];        // En cada apuntar irá un vector de tamaña ncolumas
    }
}

matriz::~matriz(){                           // Funcion para destruir la matriz de forma dinámica (Destructor)
    if (Tab==NULL){}                         // Si es NULL no se hace nada
    else{
        for( int i=0;i<nfilas;i++){          // Aqui se va eliminando cada vector
            delete[] Tab[i];
        }
        delete[] Tab;                        // Aqui se elimina el apuntador
    }
}

void matriz::generacion_aleatoria(){         // Función para rellenar la matriz con numeros aleatorios
    for (int i=0;i<nfilas;i++){              // Aqui se pasa por cada entrada
        for (int j=0;j<ncolumnas;j++){
            Tab[i][j] = rand() % 100;        // Aqui se asigna a cada entrada un número en el [0,99] enteros
        }
    }
}

void matriz::print(){                        // Función para imprimir los valores
    if (Tab==NULL){}                         // Si es NULL no hacemos nada
    else{
        for (int i=0;i<nfilas;i++){          // Aqui se recorre cada entrada
            for (int j=0;j<ncolumnas;j++){
                cout << Tab[i][j] << "\t";   // Aqui se imprime cada entrada
            }
        cout << endl;                        // Saltar de linea
        }
    }
}

