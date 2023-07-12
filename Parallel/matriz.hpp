// Autor: Yair Antonio Castillo Castillo

#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#pragma once
#include <iostream>

using namespace std;

class matriz{      // Aquí se define la clase matriz
private:
public:
    double **Tab;  // El apuntador para crear la matriz
    int nfilas;    // El numero de filas de la matriz
    int ncolumnas; // El numero de columnas de la matriz
    matriz(int filas, int columnas);   // Aqui se crea la matriz (Constructor)
    ~matriz();                         // Aqui se destruye la matriz (Destructor)
    void print();                      // Aqui se imprime la matriz
    void generacion_aleatoria();       // Aqui se llena la matriz con valores aleatorios
    matriz inversa();                 // Funcion para obtener la inversa
    matriz operator+(const matriz &A); // Funcion sumar 2 matrices (sobrecarga de operador)
    matriz operator-(const matriz &A); // Funcion restar 2 matrices (sobrecarga de operador)
    matriz operator*(const matriz &A); // Funcion mult. 2 matrices (sobrecarga de operador)
    void operator=(const matriz A); // Funcion copiar 2 matrices (sobrecarga de operador)
};

#endif /* matriz_hpp */
