#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum class Cell { Empty, Black, White };

class Board {
public:
    Board() 
    {
        board.resize(8, vector<Cell>(8, Cell::Empty));
        board[3][3] = board[4][4] = Cell::White;
        board[3][4] = board[4][3] = Cell::Black;
    }

    void display() const 
    {
        cout << "  0 1 2 3 4 5 6 7\n";
        for (int i = 0; i < 8; ++i) {
            cout << i << " ";
            for (int j = 0; j < 8; ++j) {
                char c = board[i][j] == Cell::Black ? 'B' : (board[i][j] == Cell::White ? 'W' : '.');
                cout << c << " ";
            }
            cout << "\n";
        }
    }

    bool isValidMove(int row, int col, Cell player) const {
        if (board[row][col] != Cell::Empty) return false;

        Cell opponent = (player == Cell::Black) ? Cell::White : Cell::Black; //if oponent white => black

        for (int dRow = -1; dRow <= 1; ++dRow) {
            for (int dCol = -1; dCol <= 1; ++dCol) {
                if (dRow == 0 && dCol == 0) continue;
                if (hasCapture(row, col, dRow, dCol, player, opponent)) return true;
            }
        }
        return false;
    }

    void placePiece(int row, int col, Cell player) {
        Cell opponent = (player == Cell::Black) ? Cell::White : Cell::Black;
        board[row][col] = player;

        for (int dRow = -1; dRow <= 1; ++dRow) {
            for (int dCol = -1; dCol <= 1; ++dCol) {
                if (dRow == 0 && dCol == 0) continue;
                if (hasCapture(row, col, dRow, dCol, player, opponent)) {
                    flipPieces(row, col, dRow, dCol, player, opponent);
                }
            }
        }
    }

    bool hasEmptyCells() const {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == Cell::Empty) {
                    return true;
                }
            }
        }
        return false;
    }

    bool hasValidMove(Cell player) const {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (board[i][j] == Cell::Empty && isValidMove(i, j, player)) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    vector<vector<Cell>> board;

    bool hasCapture(int row, int col, int dRow, int dCol, Cell player, Cell opponent) const {
        int r = row + dRow;
        int c = col + dCol;
        bool opponentFound = false;

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            if (board[r][c] == opponent) {
                opponentFound = true;
            }
            else if (board[r][c] == player) {
                return opponentFound;
            }
            else {
                break;
            }
            r += dRow;
            c += dCol;
        }
        return false;
    }

    void flipPieces(int row, int col, int dRow, int dCol, Cell player, Cell opponent) {
        int r = row + dRow;
        int c = col + dCol;

        while (board[r][c] == opponent) {
            board[r][c] = player;
            r += dRow;
            c += dCol;
        }
    }
};

class Player {
public:
    Player(Cell color) : playerColor(color) {}

    void makeMove(Board& board) {
        int row, col;
        string input;

        cout << "Turn Player " << (playerColor == Cell::Black ? "B" : "W") << ". Enter line and column or type 'exit' to finish game: ";
        cin >> input;

        if (input == "exit") {
            cout << "Game is over, thank you!\n";
            exit(0);
        }

        try {
            row = stoi(input);
            cin >> col;
        }
        catch (...) {
            cout << "Bad decision. Try again.\n";
            makeMove(board);
            return;
        }

        if (row < 0 || row >= 8 || col < 0 || col >= 8 || !board.isValidMove(row, col, playerColor)) {
            cout << "Bad decision. Try again.\n";
            makeMove(board);
        }
        else {
            board.placePiece(row, col, playerColor);
        }
    }

    Cell getColor() const {
        return playerColor;
    }

private:
    Cell playerColor;
};

class Game {
public:
    Game() : currentPlayer(Cell::Black), board() {}

    bool start() {
        Player playerBlack(Cell::Black);
        Player playerWhite(Cell::White);

        while (true) {
            board.display();

            if (!board.hasValidMove(currentPlayer)) {
                if (!board.hasValidMove(opponent(currentPlayer))) {
                    cout << "Game is Over!\n";
                    return false;
                }
                cout << "Player " << (currentPlayer == Cell::Black ? "B" : "W") << " pass move.\n";
                switchPlayer();
                continue;
            }

            if (currentPlayer == Cell::Black) {
                playerBlack.makeMove(board);
            }
            else {
                playerWhite.makeMove(board);
            }

            switchPlayer();
        }
    }

private:
    Board board;
    Cell currentPlayer;

    void switchPlayer() {
        currentPlayer = (currentPlayer == Cell::Black) ? Cell::White : Cell::Black;
    }

    Cell opponent(Cell player) const {
        return (player == Cell::Black) ? Cell::White : Cell::Black;
    }
};

int main() {
    

    Game game;
    if (!game.start()) {
        return 0;
    }
    return 0;
}
