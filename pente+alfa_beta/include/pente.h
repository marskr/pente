#include "player.h"

#include <array>
#include <string>

// See this page for rules:
// https://en.wikipedia.org/wiki/Pente

// Size of the board, we assume it's square
const int BoardSize = 19;
// Number of players allowed in the game. There probably will never be more.
const int NumberOfPlayers = 2;
// To win, a player needs to have at least this number of stones in a row, column, diagonal
const int NumberOfStonesInLineToWin = 5;
// To win, a player needs to capture at least this number of opponent's pairs
const int NumberOfPairsToCaptureToWin = 5;

class Pente
{
public:
    // The board is an array of arrays, but we typedef it, because it's type name is freaking long
    typedef std::array<std::array<Player::PlayerColours, BoardSize>, BoardSize > board_type;

    Pente();
    ~Pente();

    // Init the game. MUST BE to start
    void initGame();
    // Set the chosen cell to current player's colour
    void markCell(int row, int column);

    // Check if the game is won
    bool getIsWon() const;
    // Get the colour of current player. Maybe returning enum type would be better?
    std::string getCurrentPlayer() const;
    // Get the board state
    const board_type &getBoard() const;
    // Get the number of pairs captured by a player
    int getCapturedPairsOfPlayer(Player::PlayerColours colour) const;

private:
    // Check if current move captures any opponent's cell
    void checkForCapture(int row, int column);
    // Check if current move captures any opponent's cell but specyfing the direction of the check
    // d_row/d_column is a step for moving over the board. Must be -1/0/1
    void checkForCaptureInDirection(int row, int column, int d_row, int d_column);
    // Check if the player captured enough pairs to win
    bool checkForWinByCapture();
    // Check if the player has enough stones in a line
    bool checkForWinByLine(int row, int column);
    // Check if the player has enough stones in a line but specyfing the direction of the check
    // d_row/d_column is a step for moving over the board. Must be -1/0/1
    bool checkForWinByLineInDirection(int row, int column, int d_row, int d_column);

    board_type board_;
    std::array<Player, NumberOfPlayers> players_;

    Player::PlayerColours current_player_;
    Player::PlayerColours opposite_player_;

    bool is_initialized;
    bool is_won;
};
