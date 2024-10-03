#include "board.hpp"
#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
#include <limits>  // Para infinity
using namespace std;

int nodes_visited = 0;  // Contador de nodos visitados

// Función negamax
int negamax(Board& board, int B, int N, int depth, int alpha, int beta) {
    nodes_visited++;  // Incrementamos el contador de nodos visitados

    // Si alcanzamos la profundidad máxima o el juego terminó, evaluamos el tablero
    if (depth == 0 || board.endGame()) {
        return board.evaluate();
    }

    int maxEval = -std::numeric_limits<int>::infinity();  // Inicializar con -infinito

    // Generamos todos los movimientos legales para el jugador N
    for (int piece = 0; piece < 3; ++piece) {  // Asumiendo que cada jugador tiene 3 piezas
        board.selectPiece(piece);  // Seleccionamos la pieza
        
        std::vector<int> legalMoves = board.getLegalMovesAround();  // Obtener movimientos legales

        if (legalMoves.empty()) {  // Si no hay movimientos legales, el oponente ha ganado
            return -10000;  // Un valor negativo grande para penalizar esta situación
        }

        // Recorremos todos los movimientos legales
        for (int move : legalMoves) {
            // Guardamos el estado actual para poder revertirlo después del movimiento
            Board previousState = board;
            
            // Realizamos el movimiento
            board.makeMove(move);

            // Llamamos recursivamente a negamax con el tablero actualizado y el cambio de turnos
            int eval = -negamax(board, N, B, depth - 1, -beta, -alpha);

            // Restauramos el estado anterior
            board = previousState;

            // Actualizamos el valor máximo
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            // Si alfa es mayor o igual que beta, cortamos la búsqueda (poda alfa-beta)
            if (alpha >= beta) {
                break;
            }
        }
    }

    return maxEval;  // Devolvemos la mejor evaluación encontrada
}

// Función para que el jugador N haga su mejor movimiento usando negamax
void makeBestMoveForN(Board& board, int B, int N, int depth) {
    int bestEval = -std::numeric_limits<int>::infinity();
    int bestMove = -1;

    // Iteramos sobre las piezas de N para encontrar el mejor movimiento
    for (int piece = 0; piece < 3; ++piece) {
        board.selectPiece(piece);  // Seleccionamos la pieza

        std::vector<int> legalMoves = board.getLegalMovesAround();  // Obtener movimientos legales

        if (legalMoves.empty()) {
            continue;  // Si no hay movimientos legales, saltamos esta pieza
        }

        // Recorremos todos los movimientos legales
        for (int move : legalMoves) {
            // Guardamos el estado actual para revertirlo después del movimiento
            Board previousState = board;

            // Realizamos el movimiento
            board.makeMove(move);

            // Evaluamos el tablero usando negamax
            int eval = -negamax(board, B, N, depth - 1, -std::numeric_limits<int>::infinity(), std::numeric_limits<int>::infinity());

            // Restauramos el estado anterior
            board = previousState;

            // Si encontramos una mejor evaluación, actualizamos el mejor movimiento
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }

    // Si encontramos un mejor movimiento, lo realizamos
    if (bestMove != -1) {
        board.makeMove(bestMove);
    }
}

// Modificar la lógica del turno de N en el bucle del juego en main()
int main() {
    Board board;
    board.print();  // Mostrar el tablero inicial
    int contador = 0;
    std::cout << "¡Que comience el juego!" << std::endl;

    while (!board.endGame()) {
        if (board.getActiveTurn() == B) {  // Turno del jugador B (controlado por el humano)
            int direction;
            string entry;
            int pieceNumber;
            int verification = 0;
    
            while (verification == 0) {
                std::cout << "Turno del jugador B. Ingresa el número de ficha: ";
                std::cin >> entry;

                try {
                    pieceNumber = stoi(entry);
                    verification = 1;
                } catch(invalid_argument) {
                    verification = 0;
                }

                if (pieceNumber >= 1 && pieceNumber <= 3) {
                    verification = 1;
                } else {
                    verification = 0;
                    std::cout << "Número de ficha inválido. Intenta de nuevo." << std::endl;
                }
            }

            board.selectPiece(pieceNumber);  // Seleccionar la ficha según el número

            // Obtener los movimientos legales alrededor de la ficha seleccionada
            std::vector<int> legalMoves = board.getLegalMovesAround();
            if (legalMoves.empty()) {
                std::cout << "No hay movimientos legales alrededor de esta ficha. Intenta de nuevo." << std::endl;
                continue;
            }

            // Imprimir direcciones legales alrededor de la ficha seleccionada
            std::cout << "Movimientos legales alrededor de la ficha seleccionada: " << std::endl;

            for (int i = 0; i < legalMoves.size(); i++) {
                std::string directionString = board.getDirectionString(legalMoves[i]);
                std::cout << i+1 << ": " << directionString << std::endl;
            }

            std::cout << "Selecciona una dirección (1-" << legalMoves.size() << "): ";
            std::cin >> direction;

            // Verificar si la dirección seleccionada es válida
            if (direction < 1 || direction > legalMoves.size()) {
                std::cout << "Dirección inválida. Intenta de nuevo." << std::endl;
                continue;
            }else{
                contador ++;
                std::cout<<"Numero de movimientos de B: " << contador << "\n";
            }

            int selectedPosition = legalMoves[direction - 1];

            if (!board.makeMove(selectedPosition)) {
                std::cout << "Movimiento inválido. Intenta de nuevo." << std::endl;
                continue;
            }
        } else {  // Turno del jugador N (controlado por Negamax)
            std::cout << "Turno del jugador N (controlado por Negamax)." << std::endl;
            makeBestMoveForN(board, B, N, 4);  // Llamar a negamax con profundidad 4
        }

        board.print();  // Mostrar el tablero después del turno
    }

    if (board.hasBWon()) std::cout << "¡Jugador B ha ganado!" << std::endl;
    else if (board.hasNWon()) std::cout << "¡Jugador N ha ganado!" << std::endl;

    std::cout << "Nodos visitados por Negamax: " << nodes_visited << std::endl;

    return 0;
}
