#include <iostream>
#include <string>
#include "board.hpp"

Board::Board()
{
    // Bitboards para cada jugador.
    board[B] = 0b000011000001000000000000000000000000;  // Jugador B
    board[N] = 0b000000000000000000000000100000110000;  // Jugador N
    turn = B;  // Comienza jugando B
}

Board::Board(const uint64_t b, const uint64_t n, const MARK turn): board{b, n}, turn(turn) {}

Board::~Board() = default;

uint64_t Board::getBBoard() const { return board[B]; }
uint64_t Board::getNBoard() const { return board[N]; }
MARK Board::getActiveTurn() const { return turn; }

int calculateManhattanDistance(int pos1, int pos2) {
    int row1 = pos1 / 6;
    int col1 = pos1 % 6;
    int row2 = pos2 / 6;
    int col2 = pos2 % 6;
    return std::abs(row1 - row2) + std::abs(col1 - col2);
}

int Board::evaluate() {
    int scoreB = 0;  // Puntuación para el jugador B
    int scoreN = 0;  // Puntuación para el jugador N

    // Objetivos de los jugadores
    std::vector<int> targetPositionsB = { 4, 5, 11 };  // Esquinas objetivo para B
    std::vector<int> targetPositionsN = { 24, 30, 31 };     // Esquinas objetivo para N

    // Recorremos las piezas del jugador B
    for (int i = 0; i < 35; ++i) {
        if (board[B] & (oneMask << i)) {
            int closestTarget = 35;  // Inicializamos con un valor grande
            for (int target : targetPositionsB) {
                int distance = calculateManhattanDistance(i, target);
                closestTarget = std::min(closestTarget, distance);  // Guardamos la distancia más cercana
            }
            scoreB += (10 - closestTarget);  // Aumentamos la puntuación de B mientras más cerca esté
        }
    }

    // Recorremos las piezas del jugador N
    for (int i = 0; i < 35; ++i) {
        if (board[N] & (oneMask << i)) {
            int closestTarget = 35;  // Inicializamos con un valor grande
            for (int target : targetPositionsN) {
                int distance = calculateManhattanDistance(i, target);
                closestTarget = std::min(closestTarget, distance);  // Guardamos la distancia más cercana
            }
            scoreN += (10 - closestTarget);  // Aumentamos la puntuación de N mientras más cerca esté
        }
    }

    // Penalizamos a un jugador si el otro tiene piezas más cercanas a su objetivo
    for (int target : targetPositionsB) {
        int distanceB = 35, distanceN = 35;

        // Buscar la pieza más cercana de B a su objetivo
        for (int i = 0; i < 35; ++i) {
            if (board[B] & (oneMask << i)) {
                distanceB = std::min(distanceB, calculateManhattanDistance(i, target));
            }
        }

        // Buscar la pieza más cercana de N a este objetivo de B
        for (int i = 0; i < 35; ++i) {
            if (board[N] & (oneMask << i)) {
                distanceN = std::min(distanceN, calculateManhattanDistance(i, target));
            }
        }

        // Penalizar si N está más cerca del objetivo de B
        if (distanceN < distanceB) {
            scoreB -= 4;  // Penalización para B
        }
    }

    for (int target : targetPositionsN) {
        int distanceB = 35, distanceN = 35;

        // Buscar la pieza más cercana de N a su objetivo
        for (int i = 0; i < 35; ++i) {
            if (board[N] & (oneMask << i)) {
                distanceN = std::min(distanceN, calculateManhattanDistance(i, target));
            }
        }

        // Buscar la pieza más cercana de B a este objetivo de N
        for (int i = 0; i < 35; ++i) {
            if (board[B] & (oneMask << i)) {
                distanceB = std::min(distanceB, calculateManhattanDistance(i, target));
            }
        }

        // Penalizar si B está más cerca del objetivo de N
        if (distanceB < distanceN) {
            scoreN -= 4;  // Penalización para N
        }
    }

    // La función de evaluación devolverá la diferencia de puntuación entre los jugadores
    return scoreB - scoreN;
}


void Board::selectPiece(int pieceNumber) {
    uint64_t activeBoard = (turn == B) ? board[B] : board[N];  // Tablero del jugador activo
    int bitPosition = -1;
    int count = 0;
    
    // Encontrar la posición de la pieza correspondiente (1, 2 o 3)
    for (int i = 0; i < 35; i++) {
        if (activeBoard & (oneMask << i)) {
            count++;
            if (count == pieceNumber) {
                bitPosition = i;
                break;
            }
        }
    }

    if (bitPosition != -1) {
        selectedPiece = bitPosition;  // Selecciona la ficha
    } else {
        std::cout << "Número de ficha no válido." << std::endl;
        selectedPiece = -1;  // Desseleccionar si la ficha no es válida
    }
}

int Board::countPieces(MARK player) const {
    uint64_t boardMask = board[player];
    int count = 0;
    while (boardMask) {
        count += boardMask & 1;
        boardMask >>= 1;
    }
    return count;
}

std::string Board::getDirectionString(int position) const {
    int rowDiff = (position / 6) - (selectedPiece / 6);
    int colDiff = (position % 6) - (selectedPiece % 6);

    if (rowDiff == -1 && colDiff == 0) return "Arriba";
    if (rowDiff == 1 && colDiff == 0) return "Abajo";
    if (rowDiff == 0 && colDiff == -1) return "Izquierda";
    if (rowDiff == 0 && colDiff == 1) return "Derecha";
    if (rowDiff == -1 && colDiff == -1) return "Arriba-Izquierda";
    if (rowDiff == -1 && colDiff == 1) return "Arriba-Derecha";
    if (rowDiff == 1 && colDiff == -1) return "Abajo-Izquierda";
    if (rowDiff == 1 && colDiff == 1) return "Abajo-Derecha";

    return "Movimiento desconocido";
}

std::vector<int> Board::getLegalMovesAround() const {
    std::vector<int> legalMoves;
    if (selectedPiece == -1) return legalMoves;  // Si no hay ficha seleccionada, retorna vacío

    // Obtener la fila y columna actuales de la pieza seleccionada
    int row = selectedPiece / 6;
    int col = selectedPiece % 6;

    // Multiplicador de dirección para determinar las direcciones de movimiento válidas
    const std::vector<std::pair<int, int>> directions = (turn == B) ?
        std::vector<std::pair<int, int>>{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}} :
        std::vector<std::pair<int, int>>{{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

    // Recorrer las direcciones permitidas y calcular posiciones nuevas
    for (const auto& [dr, dc] : directions) {
        int newRow = row + dr;
        int newCol = col + dc;

        // Verificar si la nueva posición está dentro de los límites del tablero
        if (newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 6) {
            int newPos = newRow * 6 + newCol;

            // Si es un movimiento legal, lo añadimos a la lista
            if (isLegalMove(newPos)) {
                legalMoves.push_back(newPos);
            }
        }
    }

    return legalMoves;
}

std::vector<int> Board::getAdjacentPositions(int position) const {
    std::vector<int> adjacent;
    int row = position / 6;
    int col = position % 6;

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            // Evitar el movimiento en el mismo lugar
            if (dr == 0 && dc == 0) continue;

            int newRow = row + dr;
            int newCol = col + dc;

            // Verificar límites
            if (newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 6) {
                adjacent.push_back(newRow * 6 + newCol);
            }
        }
    }
    return adjacent;
}

bool Board::isLegalMove(const int position) const
{
    if (position < 0 || position > 35)  // posición dentro del rango?
        return false;
    if ((board[B] | board[N]) & (oneMask << position))  // posición vacía?
        return false;
    return true;
}

bool Board::makeMove(const int position) {
    if (isLegalMove(position)) {
        // Obtener el número de fichas del jugador actual
        int pieceCount = countPieces(turn);

        if (pieceCount >= 3 && selectedPiece == -1) {
            std::cout << "Debes mover una ficha existente. No puedes añadir más fichas." << std::endl;
            return false;
        }

        // Verificar si el movimiento es adyacente a una ficha propia
        std::vector<int> adjacent = getAdjacentPositions(position);
        bool isAdjacent = false;

        for (int pos : adjacent) {
            if ((board[turn] & (oneMask << pos)) != 0) {  // Verificar si hay una ficha en una posición adyacente
                isAdjacent = true;
                break;
            }
        }

        if (!isAdjacent) {
            std::cout << "Movimiento inválido. Debe moverse a una posición adyacente." << std::endl;
            return false;
        }

        // Si el jugador tiene 3 fichas, debe mover una existente
        if (pieceCount >= 3 && selectedPiece != -1) {
            // Guardar la posición anterior de la pieza
            int prevPosition = selectedPiece; 
            board[turn] &= ~(oneMask << selectedPiece);  // Remover la ficha seleccionada

            // Colocar la ficha en la nueva posición
            board[turn] |= (oneMask << position);

            // Aquí podrías agregar la lógica para permitir deshacer si es necesario
            // undoMove(prevPosition);  // Si necesitas revertir algo aquí
        }

        // Colocar la nueva ficha en la posición
        board[turn] |= (oneMask << position);

        // Alternar el turno
        turn = (turn == B) ? N : B;

        // Reiniciar la selección de ficha
        selectedPiece = -1;

        return true;
    }
    return false;
}

bool Board::checkWin(uint64_t playerBoard, MARK turn)
{
    // Las posiciones ganadoras son las esquinas opuestas
    uint64_t targetCorners[2] = {
        0b000000000000000000000000100000110000,  // Objetivo para B
        0b000011000001000000000000000000000000,  // Objetivo para N
    };
    return (playerBoard & targetCorners[turn]) == targetCorners[turn];
}

bool Board::hasBWon() const { return checkWin(board[B], B); }
bool Board::hasNWon() const { return checkWin(board[N], N); }
bool Board::isFull() const { return (board[B] | board[N]) == fullMask; }
bool Board::endGame() const { return hasBWon() || hasNWon() || isFull(); }

void Board::print() const
{
    for (int i = 0; i < 36; i++) {
        if (board[B] & (oneMask << i)) {
            int pieceNumber = __builtin_popcountll(board[B] & ((oneMask << i) - 1)) + 1;  // Determina si es B1, B2 o B3
            std::cout << "B" << pieceNumber << "  ";
        } else if (board[N] & (oneMask << i)) {
            int pieceNumber = __builtin_popcountll(board[N] & ((oneMask << i) - 1)) + 1;  // Determina si es N1, N2 o N3
            std::cout << "N" << pieceNumber << "  ";
        } else {
            std::cout << " -  ";
        }

        if (i % 6 == 5) std::cout << std::endl;  // Salto de línea cada 6 posiciones
    }
    std::cout << std::endl;
}
