// Autor: Yair Antonio Castillo Castillo

#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#pragma once
#include <iostream>

using namespace std;

class matriz{      // Aquí se define la clase matriz
private:
    int nfilas;    // El numero de filas de la matriz
    int ncolumnas; // El numero de columnas de la matriz
    double **Tab;  // El apuntador para crear la matriz
public:
    matriz(int filas, int columnas);   // Aqui se crea la matriz (Constructor)
    ~matriz();                         // Aqui se destruye la matriz (Destructor)
    void print();                      // Aqui se imprime la matriz
    void generacion_aleatoria();       // Aqui se llena la matriz con valores aleatorios
    matriz& transpuesta();             // Funcion para obtener la transpuesta
    matriz& inversa();                 // Funcion para obtener la inversa
    matriz& operator+(const matriz A); // Funcion para sumar 2 matrices (Se usa sobrecarga de operador)
    matriz& operator-(const matriz A); // Funcion para restar 2 matrices (Se usa sobrecarga de operador)
    matriz& operator*(const matriz A); // Funcion para multiplicar 2 matrices (Se usa sobrecarga de operador)
};
#endif /* matriz_hpp */
