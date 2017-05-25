#include <cmath>
#include "../include/pente.h"

Pente::Pente()
{
    is_initialized = false;
    is_won = false;
}

Pente::~Pente()
{

}

void Pente::initGame()
{
    // Initialize the board to NONE values
    std::fill(board_.begin(), board_.end(), std::array<Player::PlayerColours, BoardSize>());
    current_player_ = Player::PlayerColours::WHITE;
    opposite_player_ = Player::PlayerColours::BLACK;

    players_.at(0).setCapturedPairs(0); // At the beginning of the game, both players haven't any captured pairs
    players_.at(1).setCapturedPairs(0);

    is_initialized = true;
    is_won = false;
}

void Pente::markCell(int row, int column)
{
    // We can't play with unitialized game
    if(!is_initialized)
        throw std::runtime_error("Error! The game was not initialized!");

    // Actually we shouldn't play with unitialized game
    if(is_won)
        throw std::runtime_error("Error! The game has already been won!");

    // Needed because [] operator for array doesn't check for range validity
    if(row >= BoardSize || column >= BoardSize ||
       row < 0 || column < 0)
        throw std::runtime_error("Error! Index out of board range!");

    // We can't mark already marked cell
    if(board_.at(row).at(column) != Player::PlayerColours::NONE)
        return;

    board_[row][column] = current_player_;

    checkForCapture(row, column);

    is_won |= checkForWinByCapture();
    is_won |= checkForWinByLine(row, column);

    // dopoki nie jest wygrana to po przejsciu petli zmien
    if(!is_won)
    {
        if(current_player_ == Player::PlayerColours::WHITE)
        {
            current_player_ = Player::PlayerColours::BLACK;
            opposite_player_ = Player::PlayerColours::WHITE;
        }
        else if(current_player_ == Player::PlayerColours::BLACK)
        {
            current_player_ = Player::PlayerColours::WHITE;
            opposite_player_ = Player::PlayerColours::BLACK;
        }
    }
}

bool Pente::getIsWon() const
{
    if(!is_initialized)
        throw std::runtime_error("Error! The game was not initialized!");

    return is_won;
}

std::string Pente::getCurrentPlayer() const
{
    if(!is_initialized)
        throw std::runtime_error("Error! The game was not initialized!");

    if(current_player_ == Player::PlayerColours::BLACK)
        return "BLACK";

    if(current_player_ == Player::PlayerColours::WHITE)
        return "WHITE";

    return "WTF?!";
}

const Pente::board_type &Pente::getBoard() const
{
    if(!is_initialized)
        throw std::runtime_error("Error! The game was not initialized!");

    return board_;
}

int Pente::getCapturedPairsOfPlayer(Player::PlayerColours colour) const
{
    if(!is_initialized)
        throw std::runtime_error("Error! The game was not initialized!");

    return players_.at((int) colour - 1).getCapturedPairs();
}

void Pente::checkForCapture(int row, int column)
{
    checkForCaptureInDirection(row, column, 0, 1);
    checkForCaptureInDirection(row, column, 0, -1);

    checkForCaptureInDirection(row, column, 1, 0);
    checkForCaptureInDirection(row, column, -1, 0);

    checkForCaptureInDirection(row, column, 1, 1);
    checkForCaptureInDirection(row, column, -1, -1);

    checkForCaptureInDirection(row, column, 1, -1);
    checkForCaptureInDirection(row, column, -1, +1);
}

void Pente::checkForCaptureInDirection(int row, int column, int d_row, int d_column)
{
    // d_row/d_column should be -1/0/1
    if((d_row != 1 && d_row != 0 && d_row != -1) ||
       (d_column != 1 && d_column != 0 && d_column != -1))
        throw std::runtime_error("Direction values should be either 0 or 1 !");

    // Check if the second stone in this direction is on the board or not
    if(!(row + 3*d_row < BoardSize && column + 3*d_column < BoardSize &&
         row + 3*d_row >= 0 && column + 3*d_column >= 0))
        return;

    // Check if the second stone in this direction is current player's stone
    if(board_.at(row).at(column) == current_player_ &&
       board_.at(row + 3*d_row).at(column + 3*d_column) == current_player_)
    {
        // Check if stones between them belong to the opponent
        if(board_.at(row + d_row).at(column + d_column) == opposite_player_ &&
           board_.at(row + 2*d_row).at(column + 2*d_column) == opposite_player_)
        {
            players_.at((int)current_player_ - 1).incrementCapturedPairs();
            board_[row + d_row][column + d_column] = Player::PlayerColours::NONE;
            board_[row + 2*d_row][column + 2*d_column] = Player::PlayerColours::NONE;
        }
    }
}

bool Pente::checkForWinByCapture()
{
    return players_.at((int)current_player_ - 1).getCapturedPairs() >= NumberOfPairsToCaptureToWin;
}

bool Pente::checkForWinByLine(int row, int column)
{
    // We check 4 directions instead of 8 (like in checkForCapture) because every direction
    // must be checked in both positive and negative... well, direction
    bool is_won = false;
    is_won |= checkForWinByLineInDirection(row, column, 1, 0);
    is_won |= checkForWinByLineInDirection(row, column, 0, 1);
    is_won |= checkForWinByLineInDirection(row, column, 1, 1);
    is_won |= checkForWinByLineInDirection(row, column, 1, -1);

    return is_won;
}

bool Pente::checkForWinByLineInDirection(int row, int column, int d_row, int d_column)
{
    // d_row/d_column should be -1/0/1
    if((d_row != 1 && d_row != 0 && d_row != -1) ||
       (d_column != 1 && d_column != 0 && d_column != -1))
        throw std::runtime_error("Direction values should be either 0 or 1 !");

    int row_check = row;
    int column_check = column;

    int stones_in_line = 0;

    while(row_check < BoardSize && column_check < BoardSize &&
          row_check > 0    && column_check > 0 &&
          board_.at(row_check).at(column_check) == current_player_)
    {
        row_check += d_row;
        column_check += d_column;

        stones_in_line++;
    }

    row_check = row;
    column_check = column;

    while(row_check < BoardSize && column_check < BoardSize &&
          row_check > 0    && column_check > 0 &&
          board_.at(row_check).at(column_check) == current_player_)
    {
        row_check -= d_row;
        column_check -= d_column;

        stones_in_line++;
    }

    return (stones_in_line >= NumberOfStonesInLineToWin);
}
