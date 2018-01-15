#include <cmath>
#include <stdexcept>
#include <tuple>

#include "../include/pente.h"

Pente::Pente()
{
    is_initialized_ = false;
    is_won_ = false;
}

Pente::~Pente()
{

}

void Pente::saveToStream(std::ostream &stream)
{
    for(auto& row : board_)
    {
        for(auto& el : row)
        {
            stream << static_cast<int>(el) << " ";
        }
    }

    stream << history_.size() << " ";
    for(auto& hist_element : history_)
    {
        stream << hist_element.first.first << " ";
        stream << hist_element.first.second << " ";

        stream << hist_element.second.size() << " ";

        for(auto& hist_element_element : hist_element.second)
        {
            stream << hist_element_element.first << " ";
            stream << hist_element_element.second << " ";
        }
    }

    for(auto& player : players_)
    {
        player.saveToStream(stream);
    }

    stream << white_cells_.size() << " ";
    for(auto& white_cell : white_cells_)
    {
        stream << white_cell.first << " ";
        stream << white_cell.second << " ";
    }

    stream << black_cells_.size() << " ";
    for(auto& black_cell : black_cells_)
    {
        stream << black_cell.first << " ";
        stream << black_cell.second << " ";
    }

    stream << static_cast<int>(current_player_) << " ";
    stream << static_cast<int>(opposite_player_) << " ";

    stream << is_initialized_ << " ";
    stream << is_won_ << " ";
}

void Pente::loadFromStream(std::istream &stream)
{
    for(auto& row : board_)
    {
        for(auto& el : row)
        {
            int value;
            stream >> value;
            el = static_cast<Player::PlayerColours>(value);
        }
    }

    history_.clear();

    int history_size;
    stream >> history_size;
    for(int i = 0; i < history_size; i++)
    {
        field_type field;
        stream >> field.first;
        stream >> field.second;

        int elements;
        stream >> elements;
        std::vector<field_type> vec(elements);

        for(auto& hist_element_element : vec)
        {
            stream >> hist_element_element.first;
            stream >> hist_element_element.second;
        }

        history_.emplace_back(field, vec);
    }

    for(auto& player : players_)
    {
        player.loadFromStream(stream);
    }

    white_cells_.clear();

    int white_size;
    stream >> white_size;
    for(int i = 0; i < white_size; i++)
    {
        field_type field;
        stream >> field.first;
        stream >> field.second;

        white_cells_.insert(field);
    }


    black_cells_.clear();

    int black_size;
    stream >> black_size;
    for(int i = 0; i < black_size; i++)
    {
        field_type field;
        stream >> field.first;
        stream >> field.second;

        black_cells_.insert(field);
    }

    int current_player;
    stream >> current_player;
    current_player_ = static_cast<Player::PlayerColours>(current_player);

    int opposite_player;
    stream >> opposite_player;
    opposite_player_ = static_cast<Player::PlayerColours>(opposite_player);

    stream >> is_initialized_;
    stream >> is_won_;
}

void Pente::initGame()
{
    // Initialize the board to NONE values
    std::fill(board_.begin(), board_.end(), std::array<Player::PlayerColours, BoardSize>());
    history_.clear();

    white_cells_.clear();
    black_cells_.clear();

    current_player_ = Player::PlayerColours::WHITE;
    opposite_player_ = Player::PlayerColours::BLACK;

    players_.at(0).setCapturedPairs(0); // At the beginning of the game, both players haven't any captured pairs
    players_.at(1).setCapturedPairs(0);

    is_initialized_ = true;
    is_won_ = false;
}

void Pente::markCell(field_type cell) /* void Pente::markCell(int row, int column, bool unmark_cell) */
{
    // We can't play with unitialized game
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    // Actually we shouldn't play with unitialized game
    if(is_won_)
        throw std::runtime_error("Error! The game has already been won!");


    // We can't mark already marked cell
    if(board_.at(cell.first).at(cell.second) != Player::PlayerColours::NONE)
        return;


    board_.at(cell.first).at(cell.second) = current_player_;
    history_.emplace_back(cell, std::vector<field_type>());
    addCell(current_player_, cell);

    checkForCapture(cell);

    is_won_ |= checkForWinByCapture();
    is_won_ |= checkForWinByLine(cell);

    // dopoki nie jest wygrana to po przejsciu petli zmien
    if(!is_won_)
    {
        nextPlayer();
    }
}

void Pente::undoLastMove()
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    if(history_.empty())
    {
        return;
    }

    field_type added_cell;
    std::vector<field_type> removed_cells;

    std::tie(added_cell, removed_cells) = history_.back();
    history_.pop_back();

    if(is_won_)
    {
        is_won_ = false;
    }
    else
    {
        previousPlayer();
    }

    board_.at(added_cell.first).at(added_cell.second) = Player::PlayerColours::NONE;
    removeCell(current_player_, added_cell);
    for(const field_type cell : removed_cells)
    {
        board_.at(cell.first).at(cell.second) = opposite_player_;
        addCell(opposite_player_, cell);
    }

    for(int i = 0; i < removed_cells.size()/2; i++)
    {
        players_.at((int)current_player_ - 1).decrementCapturedPairs();
    }
}

bool Pente::getIsWon() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return is_won_;
}

Player::PlayerColours Pente::getCurrentPlayer() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return current_player_;
}

std::string Pente::getStringCurrentPlayer() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    if(current_player_ == Player::PlayerColours::BLACK)
        return "BLACK";

    if(current_player_ == Player::PlayerColours::WHITE)
        return "WHITE";

    return "WTF?!";
}

const Pente::board_type &Pente::getBoard() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return board_;
}

int Pente::getCapturedPairsOfPlayer(Player::PlayerColours colour) const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return players_.at((int) colour - 1).getCapturedPairs();
}

std::set<Pente::field_type> Pente::getWhiteCells() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return white_cells_;
}

std::set<Pente::field_type> Pente::getBlackCells() const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return black_cells_;
}

Player::PlayerColours Pente::getCellValue(Pente::field_type cell) const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return board_.at(cell.first).at(cell.second);
}

bool Pente::checkIfCellLiesInBoard(Pente::field_type cell) const
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return (cell.first >= 0 && cell.second >= 0 &&
            cell.first < BoardSize && cell.second < BoardSize);
}

void Pente::checkForCapture(field_type cell)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    for(const field_type& direction : FIELD_DIRECTIONS_)
    {
        checkForCaptureInDirection(cell, direction);
        checkForCaptureInDirection(cell, ZERO_FIELD_ - direction);
    }
}

void Pente::checkForCaptureInDirection(field_type cell, field_type direction)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    // d_row/d_column should be -1/0/1
    if((std::abs(direction.first) != 1 && direction.first != 0) ||
      (std::abs(direction.second) != 1 && direction.second != 0))
        throw std::runtime_error("Direction values should be either 0 or 1 !");

    // Check if the second stone in this direction is on the board or not
    if(!(checkIfCellLiesInBoard(cell + 3*direction)))
        return;

    // Check if the second stone in this direction is current player's stone

    field_type cell1 = cell + direction;
    field_type cell2 = cell + 2*direction;
    field_type cell3 = cell + 3*direction;

    if(board_.at(cell.first).at(cell.second) == current_player_ &&
       board_.at(cell3.first).at(cell3.second) == current_player_)
    {
        // Check if stones between them belong to the opponent
        if(board_.at(cell1.first).at(cell1.second) == opposite_player_ &&
           board_.at(cell2.first).at(cell2.second) == opposite_player_)
        {
            players_.at((int)current_player_ - 1).incrementCapturedPairs();
            board_.at(cell1.first).at(cell1.second) = Player::PlayerColours::NONE;
            board_.at(cell2.first).at(cell2.second) = Player::PlayerColours::NONE;

            removeCell(opposite_player_, cell1);
            removeCell(opposite_player_, cell2);

            history_.back().second.push_back(cell1);
            history_.back().second.push_back(cell2);
        }
    }
}

bool Pente::checkForWinByCapture()
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    return players_.at((int)current_player_ - 1).getCapturedPairs() >= NumberOfPairsToCaptureToWin;
}

bool Pente::checkForWinByLine(field_type cell)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    // We check 4 directions instead of 8 (like in checkForCapture) because every direction
    // must be checked in both positive and negative... well, direction
    bool is_won = false;
    for(const field_type& direction : FIELD_DIRECTIONS_)
    {
        is_won |= checkForWinByLineInDirection(cell, direction);
    }

    return is_won;
}

bool Pente::checkForWinByLineInDirection(field_type cell, field_type direction)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    // d_row/d_column should be -1/0/1
    if((std::abs(direction.first) != 1 && direction.first != 0) ||
      (std::abs(direction.second) != 1 && direction.second != 0))
        throw std::runtime_error("Direction values should be either 0 or 1 !");

    field_type check = cell;

    int stones_in_line = 0;

    while(checkIfCellLiesInBoard(check) &&
          board_.at(check.first).at(check.second) == current_player_)
    {
        check = check + direction;

        stones_in_line++;
    }

    check = cell - direction;

    while(checkIfCellLiesInBoard(check) &&
          board_.at(check.first).at(check.second) == current_player_)
    {
        check = check - direction;

        stones_in_line++;
    }

    return (stones_in_line >= NumberOfStonesInLineToWin);
}

void Pente::addCell(Player::PlayerColours player, Pente::field_type cell)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    if(player == Player::PlayerColours::WHITE)
    {
        white_cells_.insert(cell);
    }
    else if(player == Player::PlayerColours::BLACK)
    {
        black_cells_.insert(cell);
    }
    else
    {
        throw std::invalid_argument("Argument \"player\" is invalid!");
    }
}

void Pente::removeCell(Player::PlayerColours player, Pente::field_type cell)
{
    if(!is_initialized_)
        throw std::runtime_error("Error! The game was not initialized!");

    if(player == Player::PlayerColours::WHITE)
    {
        white_cells_.erase(cell);
    }
    else if(player == Player::PlayerColours::BLACK)
    {
        black_cells_.erase(cell);
    }
    else
    {
        throw std::invalid_argument("Argument \"player\" is invalid!");
    }
}

void Pente::nextPlayer()
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

void Pente::previousPlayer()
{
    nextPlayer();
}
