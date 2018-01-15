#ifndef PENTE_H
#define PENTE_H

#include <array>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <iostream>

#include "player.h"

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
    typedef std::pair<int, int> field_type;
    // The board is an array of arrays, but we typedef it, because it's type name is freaking long
    typedef std::array<std::array<Player::PlayerColours, BoardSize>, BoardSize > board_type;
    // The history contains a list of pairs where the first element is the new cell and
    // the second are the cells that were removed in that move (e.g. captured cells)
    typedef std::list<std::pair<field_type, std::vector<field_type> > > history_type;

    Pente();
    ~Pente();

    void saveToStream(std::ostream &stream);
    void loadFromStream(std::istream& stream);

    // Init the game. MUST BE to start
    void initGame();

    // Set the chosen cell to current player's colour
    void markCell(field_type cell);

    // Undo the last move
    void undoLastMove();

    // Check if the game is won
    bool getIsWon() const;

    // Get the colour of current player
    Player::PlayerColours getCurrentPlayer() const;

    // Get the colour of current player
    std::string getStringCurrentPlayer() const;

    // Get the board state
    const board_type &getBoard() const;

    // Get the number of pairs captured by a player
    int getCapturedPairsOfPlayer(Player::PlayerColours colour) const;


    // Get all the white cells
    std::set<field_type> getWhiteCells() const;

    // Get all the black cells
    std::set<field_type> getBlackCells() const;

    // Get the value of a cell
    Player::PlayerColours getCellValue(field_type cell) const;

    // Check if the cell lies withing board's limits
    bool checkIfCellLiesInBoard(field_type cell) const;

private:
    // Check if current move captures any opponent's cell
    void checkForCapture(field_type cell);

    // Check if current move captures any opponent's cell but specyfing the direction of the check
    // d_row/d_column is a step for moving over the board. Must be -1/0/1
    void checkForCaptureInDirection(field_type cell, field_type direction);

    // Check if the player captured enough pairs to win
    bool checkForWinByCapture();

    // Check if the player has enough stones in a line
    bool checkForWinByLine(field_type cell);

    // Check if the player has enough stones in a line but specyfing the direction of the check
    // d_row/d_column is a step for moving over the board. Must be -1/0/1
    bool checkForWinByLineInDirection(field_type cell, field_type direction);

    // This function serves to make containing black and white cells easier
    void addCell(Player::PlayerColours player, field_type cell);

    // This function serves to make containing black and white cells easier
    void removeCell(Player::PlayerColours player, field_type cell);

    // Moves current_player_ and opposite_player_ forth
    void nextPlayer();

    // The same as nextPlayer(), but implemented to reduce confusion
    void previousPlayer();


    board_type board_;
    history_type history_;

    std::array<Player, NumberOfPlayers> players_;

    const field_type ZERO_FIELD_ = std::make_pair(0, 0);
    std::set<field_type> white_cells_;
    std::set<field_type> black_cells_;
    const std::array<field_type, 4> FIELD_DIRECTIONS_ = {{
        std::make_pair(1, 0),
        std::make_pair(0, 1),
        std::make_pair(1, 1),
        std::make_pair(1, -1)
    }};

    Player::PlayerColours current_player_;
    Player::PlayerColours opposite_player_;

    bool is_initialized_;
    bool is_won_;
};

inline Pente::field_type operator+(const Pente::field_type& f1, const Pente::field_type& f2)
{
    return std::make_pair(f1.first + f2.first, f1.second + f2.second);
}

inline Pente::field_type operator-(const Pente::field_type& f1, const Pente::field_type& f2)
{
    return std::make_pair(f1.first - f2.first, f1.second - f2.second);
}

inline Pente::field_type operator*(const Pente::field_type& f, const int& i)
{
    return std::make_pair(f.first * i, f.second * i);
}

inline Pente::field_type operator *(const int& i, const Pente::field_type& f)
{
    return f * i;
}

#endif // PENTE_H
