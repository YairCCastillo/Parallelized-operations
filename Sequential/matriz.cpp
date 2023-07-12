// Autor: Yair Antonio Castillo Castillo

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "matriz.hpp"

matriz& matriz::operator+(const matriz M){   // Funcion para sumar matrices (Se usa sobrecarga de operador)
    matriz *M1;                              // Aqui se crea matriz extra donde se guardara el resultado de la suma
    M1 = new matriz(nfilas,ncolumnas);
    if (nfilas!=M.nfilas || ncolumnas!=M.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
        cout << "Las dimensiones no son iguales por lo tanto no se pueden sumar" << endl; // Aqui se regresa un mensaje de error
        M1->Tab=NULL;                        // en caso de que las dimensiones no coincidan
        return *M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<M.nfilas;i++){           // En cada entrada de la matriz creada se suma las entradas de las matrices
        for(int j=0;j<M.ncolumnas;j++){
            M1->Tab[i][j] = (this->Tab)[i][j]+M.Tab[i][j];
        }
    }
    cout << "La suma de las matrices es:" << endl; // Aquí se regresa un mensaje
    return *M1;                              // Aqui se regresa la matriz que es la suma de las matrices
}

matriz& matriz::operator-(const matriz M){   // Funcion para restar matrices (Se usa sobrecarga de operador)
    matriz *M1;                              // Aqui se crea matriz extra donde se guardara el resultado de la resta
    M1 = new matriz(nfilas,ncolumnas);
    if (this->nfilas!=M.nfilas || this->ncolumnas!=M.ncolumnas ){  // Aqui se checa si cumplen que las dimensiones son iguales
        cout << "Las dimensiones no son iguales por lo tanto no se puede restar" << endl; // Aqui se regresa un mensaje de error
        M1->Tab=NULL;                        // en caso de que las dimensiones no coincidan
        return *M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<M.nfilas;i++){           // En cada entrada de la matriz creada restamos las entradas de las matrices
        for(int j=0;j<M.ncolumnas;j++){
            M1->Tab[i][j] = Tab[i][j]-M.Tab[i][j];
        }
    }
    cout << "La resta de las matrices es:" << endl; // Aquí se regresa un mensaje
    return *M1;                              // Aquí se regresa la matriz que es la resta de las matrices
}

matriz& matriz::operator*(const matriz M){   // Funcion para multiplicar matrices (Se usa sobrecarga de operador)
    matriz *M1;                              // Aqui se crea matriz extra donde se guardara el resultado de la multiplicación
    M1 = new matriz(nfilas,M.ncolumnas);
    if (this->ncolumnas!=M.nfilas){          // Aqui se checa si el # de columnas de la 1º matriz coincide con el # de renglones de la 2º matriz
        cout << "El numero de columnas de la 1º matriz no coicididen con el numero de renglonoes de la 2º matriz, por lo tanto no se pueden multiplicar" << endl;
        M1->Tab=NULL;                        //  Aqui se regresa un mensaje de error en caso de que las dimensiones no coincidan
        return *M1;                          // Aqui se regresa un apuntador doble igua a NULL
    }
    for (int i=0; i<nfilas;i++){             // En cada entrada de la matriz creada se multiplica el renglón de la 1º matriz
        for(int j=0;j<M.ncolumnas;j++){      // por la columna de la 2º matriz
            int y=0;                         // Aqui guardamos la suma de la multiplicación
            for (int k=0;k<(this->ncolumnas);k++){
                y=y+Tab[i][k]*M.Tab[k][j];
            }
            M1->Tab[i][j] = y;               // Aqui se asigna a cada entrada la suma
        }
    }
    cout << "La multiplicacion de las matrices es:" << endl;
    return *M1;                              // Aqui se regresa la matriz que es la multiplicacion de las matrices
    
}

matriz& matriz::transpuesta(){               // Funcion para obtener la transpuesta
    matriz *M1;                              // Se crea una matriz extra donde se guardara la transpuesta con las dimensiones invertidas
    M1 = new matriz(ncolumnas,nfilas);
    for (int i=0;i<ncolumnas;i++){           // En cada entrada de la matriz creada se inserta las coordenadas invertidas de la matriz
        for (int j=0;j<nfilas;j++){
            M1->Tab[i][j] = Tab[j][i];
        }
    }
    cout << "La matriz transpuesta es:" << endl; // Aqui se regresa un mensaje
    return *M1;                              // Aqui se regresa la matriz transpuesta
}
    
matriz& matriz::inversa(){                   // Funcion para obtener la inversa usando Gauss-Jordan
    matriz *Inver;
    Inver = new matriz(nfilas,ncolumnas);    // Aqui se crea una matriz extra donde se guardara la inversa con las dimensiones cambiadas
    if (ncolumnas!=nfilas){                  // Aqui se checa si la matriz es cuadrada para ver si podemos obtener la inversa
        cout << "No se puede obtener la inversa, las matriz tiene que ser cuadrada" << endl; // Aqui se regresa un mensaje de error
        cout << endl;                        // en caso de que la matriz no es cuadrada
        Inver->Tab=NULL;                     // Aqui se regresa un apuntador doble igua a NULL
        return *Inver;
    }
    for (int i=0; i<nfilas;i++){             // Para obterner la inversa se tiene que crear una matriz identidad cuadrada y poder aplicar
        for(int j=0;j<ncolumnas;j++){        // Gauss-Jordan
            Inver->Tab[i][j] = (double) 0;   // 0 en todas las entradas
            if (i==j){
                Inver->Tab[i][j] =  (double) 1; // 1 en la diagonal
            }
        }
    }
    for (int i=0;i<(nfilas);i++){            // Aqui se empieza haciendo la matriz escalonado por renglones, donde se ira eliminando todos
        double a = Tab[i][i];                // los elementos que estén debajo de la diagonal
        int k=0;
        while (a==0){                        // Si el elemento de la diagonal es 0, ver que exista un elemento abajo de esa diagonal que no sea 0
            for (k=i;k<nfilas;k++){
                a = Tab[k][i];               // Ver cada una
            }
            if (k==(nfilas)){                // En caso no es existir, entonces regresar error de que no existe inversa para esa matriz
                cout << "La matriz no es invertible " << endl;
                cout << endl;
                Inver->Tab=NULL;             // Aqui se regresa un apuntador doble igua a NULL
                return *Inver;
            }
        }
        if (k>i){                            // Si la diagonal fue 0 y existe otro valor abajo que no es 0, entonces se intercambian los renglones
            double aux = *Tab[k];            // Se intercambian para la matriz original
            Tab[k]=Tab[i];
            *Tab[i] = aux;
            double aux1 = *Inver->Tab[k];    // Y tambien se cambia en la matriz identidad (Ya modificada si no es la primera interacion)
            Inver->Tab[k]=Inver->Tab[i];
            *Inver->Tab[i] = aux1;
        }
        for (int p=i+1;p<nfilas;p++){        // Aqui se empieza a eliminar los elementos abajo de la diagonal y tambien se modifica la matriz identidad
            double b = Tab[p][i];            // Haciendo a la matriz origianl un triangular superior y a la identidad un triangular inferior
            for (int j=0;j<ncolumnas;j++){
                Tab[p][j] = (double) Tab[p][j] - (double)Tab[i][j]*(double)b/(double)a;
                Inver->Tab[p][j] = (double)Inver->Tab[p][j]-(double)Inver->Tab[i][j]*(double)b/(double)a;
            }
        }
    }

    for (int i=(nfilas-1); i>=0;i--){        // Aqui se empieza eliminar los elementos que estan arriba de la diagonal
        for (int k=0;k<nfilas;k++){          // Aqui se divide cada renglón en la matriz identidad por el elemento de la diagonal de la matriz
            Inver->Tab[i][k]=Inver->Tab[i][k]/Tab[i][i];
        }
        Tab[i][i]=1;                         // Aqui se hace la diagonal igual a 1, después de que en la matriz identidad se divide por el valor de
        double a = Tab[i][i];                // la diagonal
        for (int p=(i-1);p>=0;p--){          // Aqui se eliminan los elementos arriba de la diagonal de la matriz original y se rellenan los
            double b = Tab[p][i];            // lo valores de la matriz indentidad
            for (int j=0;j<ncolumnas;j++){
                Tab[p][j] = (double) Tab[p][j] - (double)Tab[i][j]*(double)b/(double)a;
                Inver->Tab[p][j] = (double) Inver->Tab[p][j] - (double)Inver->Tab[i][j]*(double)b/(double)a;
            }
        }
        
    }
    cout << "La matriz inversa es:" << endl; // Aqui se imprime un mensaje
    return *Inver;                           // Regresamos la matriz transpuesta
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
