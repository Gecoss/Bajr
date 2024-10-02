#include "evaluationFunction.hpp"
#include <cmath> 

// Función para calcular la distancia vertical entre dos posiciones en un tablero 6x6
int calculateVerticalDistance(int pos1, int pos2) {
    int row1 = pos1 / 6;
    int row2 = pos2 / 6;
    return std::abs(row1 - row2);
}

