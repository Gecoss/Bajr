#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <string>
#include <vector>

enum MARK {B, N};  // Hasta 2 jugadores

class Board
{
    uint64_t board[2]{};  // Sección para 2 jugadores
    MARK turn;

    uint64_t oneMask = 0b000000000000000000000000000000000001;  // 1 en la posición 0
    uint64_t fullMask = 0b111111111111111111111111111111111111;  // 36 bits

    bool isLegalMove(int position) const;  // Verificar si la posición es legal
    private: static bool checkWin(uint64_t board, MARK turn);  // Pasar el turno como argumento
    int countPieces(MARK player) const;


    public:
        static const int EMPTY = -1;
        void selectPiece(int position);  // Método para seleccionar una ficha
        std::vector<int> getLegalMovesAround() const;  // Método para obtener movimientos legales alrededor de la ficha seleccionada
        Board();  // Constructor prototype
        Board(uint64_t x, uint64_t o, MARK turn);
        ~Board();  // Destructor prototype

        int evaluate();  // Función de evaluación
        std::vector<int> generateAllLegalMoves() const;
        std::vector<int> getAdjacentPositions(int position) const;
        std::string getDirectionString(int position) const;

        uint64_t getBBoard() const;
        uint64_t getNBoard() const;
        MARK getActiveTurn() const;

        void print() const;  // Función para imprimir el tablero en una forma legible
        bool makeMove(int position);  // Función para hacer una jugada

        bool hasBWon() const;  // Verificar si X ha ganado
        bool hasNWon() const;  // Verificar si O ha ganado
        bool isFull() const;  // Verificar si el tablero está lleno
        bool endGame() const;

        bool operator==(const Board &other) const {
            return turn==other.turn && board[B]==other.board[B] && board[N]==other.board[N];
        }
    private:
        int selectedPiece = -1;  // Índice de la ficha seleccionada

};

struct BoardHash
{
    std::size_t operator()(Board const& board) const noexcept
    {
        std::size_t h1 = std::hash<uint64_t>{}(board.getBBoard());
        std::size_t h2 = std::hash<uint64_t>{}(board.getNBoard());
        std::size_t h3 = std::hash<bool>{}(board.getActiveTurn() == B);

        // Mezclar usando XOR y desplazamientos
        return h1 ^ (h2 << 1) ^ (h2 >> 1) ^ (h3 << 2) ^ (h3 >> 2);
    }
};

#endif //BOARD_HPP
