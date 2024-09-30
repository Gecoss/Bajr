#include "board.hpp"
#include <iostream>
#include <vector>
#include <algorithm> 

int main() {
    Board board;
    board.print();  // Mostrar el tablero inicial

    std::cout << "¡Que comience el juego!" << std::endl;

    while (!board.endGame()) {
        int direction;
        int pieceNumber;
        std::cout << "Turno del jugador " << (board.getActiveTurn() == B ? "B" : "N")
                << ". Ingresa el número de ficha: ";
        std::cin >> pieceNumber;

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
        }

        int selectedPosition = legalMoves[direction - 1];

        if (!board.makeMove(selectedPosition)) {
            std::cout << "Movimiento inválido. Intenta de nuevo." << std::endl;
            continue;
        }
        board.print();
    }

    if (board.hasBWon()) std::cout << "¡Jugador B ha ganado!" << std::endl;
    else if (board.hasNWon()) std::cout << "¡Jugador N ha ganado!" << std::endl;

    return 0;
}
