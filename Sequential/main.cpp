
// Autor: Yair Antonio Castillo Castillo
//
// Descripcion: Codigo para realizar las siguientes operaciones del algebra de matrices: (SUMA, RESTA, MULTIPLICACION, TRANSPUESTA, INVERSA),
// donde en la inversa se tiene que hacer mediante el uso de eliminacion Gauss-Jordan. Para las matrices y su tamaño son guardadas en una clase,
// y estas son llenadas de manera aleatoria con numeros en el intervalo [0,99] usando srand para evitar que se generan matrices identicas o matrices
// con muchos elementos identidos ademas que ser manejadas mediante memoria dinamica. Las operaciones generan una matriz extra y retornan los
// resultados correspondientes, para las operaciones para la suma, resta y multiplicacion se usa sobrecarga de operaciones (+,-,*) respectivamente
// Para cada de las operaciones se consideran las condiciones para poder ser realizadas, para la suma, resta las matrices tiene que tener las mismas
// condiciones, para la multiplicación que coincidan las columnas de la 1º matriz con los renglones de la 2º matriz, para la transpuesta no hay
// condicionas y para la inversa tiene que ser cuadradas, de otra manera se regresará la matriz con contenido igual a NULL.
// Se harán 3 archivos: "main.cpp" donde esta el main, "matriz.cpp" donde estan todas las funciones y "matriz.hpp" donde esta la clase.

#include <iostream>
#include "matriz.cpp"
#include <time.h>

int main() {// La función principial donde se dará un menu que se repetira cada que se una accian
    int opcion,n1,m1,n2,m2;
    srand((unsigned int)time(NULL));         // Para obtener número aleatorios diferentes en las matrices
    do{                                      // Las operaciones de las matrices
        matriz *A;                           // Se Inicializan las matrices
        matriz *B;
        matriz *C = new matriz(0,0);         // Para que no regrese warning de inicializar
        cout << "Elige la opcion de la operacion de algebra de matrices que se desea hacer: " << endl;
        cout << "1. Sumar 2 matrices" << endl;
        cout << "2. Restar 2 matrices" << endl;
        cout << "3. Multiplicar 2 matrices" << endl;
        cout << "4. Obtener la transpuesta" << endl;
        cout << "5. Obterner la inversa" << endl;
        cout << "6. Salir" << endl;
        std::cout << "Opción:";
        std::cin >> opcion;
        switch(opcion){
            case 1:{    // La primer operación es la suma, lo cual pedirá que inserte las dimensiones de las 2 matrices
                std::cout << "Inserte el numero de renglones de la 1º matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la 1º matriz: ";
                std::cin >> m1;
                std::cout << "Inserte el numero de renglones de la 2º matriz: ";
                std::cin >> n2;
                std::cout << "Inserte el numero de columnas de la 2º matriz: ";
                std::cin >> m2;
                cout << endl;
                A = new matriz(n1,m1);     // Se crean las matrices
                B = new matriz(n2,m2);
                A->generacion_aleatoria(); // Se llenan de números aleatorios
                B->generacion_aleatoria();
                cout << "La 1º matriz es:" << endl;
                A->print();                // Se imprimen las matrices
                cout << endl;
                cout << "La 2º matriz es:" << endl;
                B->print();
                cout << endl;
                *C = *A + *B;              // Se usa el operador "+"
                C->print();                // Se imprime la matriz de la resta de la matrices
                cout << endl;
                delete A;                  // Se eliminan las matrices
                delete C;
                break;
            }
            case 2:{  // La segunda operación es la resta, lo cual pedirá que inserte las dimensiones de las 2 matrices
                cout << "Se restara la 1º matriz menos la 2º matriz (en ese orden) " << endl;
                std::cout << "Inserte el numero de renglones de la 1º matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la 1º matriz: ";
                std::cin >> m1;
                std::cout << "Inserte el numero de renglones de la 2º matriz: ";
                std::cin >> n2;
                std::cout << "Inserte el numero de columnas de la 2º matriz: ";
                std::cin >> m2;
                cout << endl;
                A = new matriz(n1,m1);      // Se crean las matrices
                B = new matriz(n2,m2);
                A->generacion_aleatoria();  // Se llenan de números aleatorios
                B->generacion_aleatoria();
                cout << endl;
                cout << "La 1º matriz es:" << endl;
                A->print();                 // Se imprimen las matrices
                cout << endl;
                cout << "La 2º matriz es:" << endl;
                B->print();
                cout << endl;
                *C = *A - *B;               // Se usa el operador "-"
                C->print();                 // Se imprime la matriz de la resta de la matrices
                cout << endl;
                delete A;                   // Se eliminan las matrices
                delete C;
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
                A = new matriz(n1,m1);      // Se crean las matrices
                B = new matriz(n2,m2);
                A->generacion_aleatoria();  // Se llenan de números aleatorios
                B->generacion_aleatoria();
                cout << "La 1º matriz es:" << endl;
                A->print();                 // Se imprimen las matrices
                cout << endl;
                cout << "La 2º matriz es:" << endl;
                B->print();
                cout << endl;
                *C = *A * *B;               // Se usa el operador "*"
                C->print();                 // Se imprime la matriz de la multiplicación de la matrices
                cout << endl;
                delete A;                   // Se eliminan las matrices
                delete C;
                break;
            }
            case 4:{   // La cuarta operación es la transpuesta, lo cual pedirá que inserte las dimension de la matriz
                std::cout << "Inserte el numero de renglones de la matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la matriz: ";
                std::cin >> m1;
                cout << endl;
                A = new matriz(n1,m1);      // Se crea la matriz
                A->generacion_aleatoria();  // Se llena de números aleatorios
                cout << "La matriz es:" << endl;
                A->print();                 // Se imprimen la matriz
                cout << endl;
                *C = A->transpuesta();      // Se usa la función transpuesta
                C->print();                 // Se imprime la matriz transpuesta
                cout << endl;
                delete A;                   // Se eliminan las matrices
                delete C;
                break;
            }
            case 5:{    // La quinta operación es la transpuesta, lo cual pedirá que inserte las dimension de la matriz
                std::cout << "Inserte el numero de renglones de la matriz: ";
                std::cin >> n1;
                std::cout << "Inserte el numero de columnas de la matriz: ";
                std::cin >> m1;
                cout << endl;
                A = new matriz(n1,m1);      // Se crea la matriz
                A->generacion_aleatoria();  // Se llena de números aleatorios
                cout << "La matriz es:" << endl;
                A->print();                 // Se imprime la matriz
                cout << endl;
                *C = A->inversa();          // Se usa la función inversa
                C->print();                 // Se imprime la matriz inversa
                cout << endl;
                delete A;                   // Se eliminan las matrices
                delete C;
                break;
            }
        }
        cout << endl;
    }
    while(opcion<6);
    printf("Gracias por usar este programa\n");
    return 0;
}
