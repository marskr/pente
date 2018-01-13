#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>

#include "../include/algorithm.h"
#include "../include/value.h"

PenteEvaluation::PenteEvaluation(Player::PlayerColours playing_player,
                                 float player_line_weigth, float opponent_line_weigth,
                                 float player_pair_weigth, float opponent_pair_weigth) :
    PLAYING_PLAYER_(playing_player),
    PLAYER_LINE_WEIGTH_(player_line_weigth),
    OPPONENT_LINE_WEIGTH_(opponent_line_weigth),
    PLAYER_PAIR_WEIGTH_(player_pair_weigth),
    OPPONENT_PAIR_WEIGTH_(opponent_pair_weigth)
{
    if(PLAYING_PLAYER_ == Player::PlayerColours::WHITE)
    {
        player_to_colour_.emplace(PlayerType::PLAYER,
                                  Player::PlayerColours::WHITE);

        player_to_colour_.emplace(PlayerType::OPPONENT,
                                  Player::PlayerColours::BLACK);
    }
    else if (PLAYING_PLAYER_ == Player::PlayerColours::BLACK)
    {
        player_to_colour_.emplace(PlayerType::PLAYER,
                                  Player::PlayerColours::BLACK);

        player_to_colour_.emplace(PlayerType::OPPONENT,
                                  Player::PlayerColours::WHITE);
    }
    else
    {
        throw std::invalid_argument("This playing player does not exist!");
    }
}

PenteEvaluation::~PenteEvaluation()
{

}

float PenteEvaluation::evaluate(const Pente &game_state)
{
    float evaluation = 0.0;
    if(game_state.getIsWon())
    {
        evaluation = std::numeric_limits<float>::infinity();
        if(game_state.getCurrentPlayer() != PLAYING_PLAYER_)
        {
            evaluation = -evaluation;
        }
    }
    else
    {
        std::vector<int> player_lengths = checkForLinesByPlayerType(game_state,
                                                                PlayerType::PLAYER);

        int player_captures = game_state.getCapturedPairsOfPlayer(player_to_colour_.at(PlayerType::PLAYER));
//        std::cout << "Player captures: " << player_captures << std::endl;

        std::vector<int> opponent_lengths = checkForLinesByPlayerType(game_state,
                                                                  PlayerType::OPPONENT);

        int opponent_captures = game_state.getCapturedPairsOfPlayer(player_to_colour_.at(PlayerType::OPPONENT));
//        std::cout << "Opponent captures: " << opponent_captures << std::endl;


//        std::cout << "Player lengths: ";
        for(const int& length : player_lengths)
        {
            evaluation += PLAYER_LINE_WEIGTH_ * std::pow(length, 2);
//            std::cout << length << " ";
        }
//        std::cout << std::endl;
        evaluation += PLAYER_PAIR_WEIGTH_ * std::pow(player_captures, 2);

//        std::cout << "Opponent lengths: ";
        for(const int& length : opponent_lengths)
        {
            evaluation -= OPPONENT_LINE_WEIGTH_ * std::pow(length, 2);
//            std::cout << length << " ";
        }
//        std::cout << std::endl;
        evaluation -= OPPONENT_PAIR_WEIGTH_ * std::pow(opponent_captures, 2);
    }

//    std::cout << "Eval : " << evaluation << std::endl;

    return evaluation;
}

std::tuple<int, int, int, int> PenteEvaluation::getAreaOfCare(const Pente &game_state)
{
//    std::tuple<int, int, int, int> one = getBordersForSet(game_state.getWhiteCells());
//    std::tuple<int, int, int, int> two = getBordersForSet(game_state.getBlackCells());

//    std::tuple<int, int, int, int> ret_value = std::make_tuple(std::min(std::get<0>(one), std::get<0>(two)),
//                                                               std::max(std::get<1>(one), std::get<1>(two)),
//                                                               std::min(std::get<2>(one), std::get<2>(two)),
//                                                               std::max(std::get<3>(one), std::get<3>(two)));

//    std::get<0>(ret_value) = std::max(0, std::get<0>(ret_value) - AreaOfCareExtend);
//    std::get<1>(ret_value) = std::min(BoardSize - 1, std::get<1>(ret_value) + AreaOfCareExtend);
//    std::get<2>(ret_value) = std::max(0, std::get<2>(ret_value) - AreaOfCareExtend);
//    std::get<3>(ret_value) = std::min(BoardSize - 1, std::get<3>(ret_value) + AreaOfCareExtend);

//    return ret_value;
    return std::make_tuple(0, 8, 0, 8);
}

std::vector<int> PenteEvaluation::checkForLinesByPlayerType(const Pente &game_state,
                                                        const PlayerType player)
{
    std::set<Pente::field_type> cells;
    if(player_to_colour_.at(player) == Player::PlayerColours::WHITE)
    {
        cells = game_state.getWhiteCells();
    }
    else
    {
        cells = game_state.getBlackCells();
    }

    std::vector<int> lengths;
    std::vector<Pente::field_type> directions = {
        std::make_pair(1, 0),
        std::make_pair(0, 1),
        std::make_pair(1, 1),
        std::make_pair(1, -1)
    };
    for(const Pente::field_type& cell : cells)
    {

        bool is_single = true;
        for(const Pente::field_type& direction : directions)
        {
            int length = checkForLineAtCellWithDirection(game_state,
                                                         cell,
                                                         direction,
                                                         player);

            if(length > 1)
            {
                lengths.push_back(length);
            }
            if(length != 1)
            {
                is_single = false;
            }
        }

        if(is_single)
        {
            lengths.push_back(1);
        }
    }

    return lengths;
}

int PenteEvaluation::checkForLineAtCellWithDirection(const Pente &game_state,
                                             const Pente::field_type &cell,
                                             const Pente::field_type &direction,
                                             const PlayerType player)
{
    if(game_state.checkIfCellLiesInBoard(cell - direction))
    {
        // If the cell behind has the same colour then it means we've already found this line
        if(game_state.getCellValue(cell - direction) == player_to_colour_.at(player))
        {
            return 0;
        }
    }

    int length = 1;
    Pente::field_type tmp = cell + direction;
    while(game_state.checkIfCellLiesInBoard(tmp) &&
          game_state.getCellValue(tmp) == player_to_colour_.at(player))
    {
        tmp = tmp + direction;
        length++;
    }

    return length;
}

std::tuple<int, int, int, int> PenteEvaluation::getBordersForSet(const std::set<Pente::field_type> &cells)
{
    int lower_first = BoardSize, upper_first = -1, lower_second = BoardSize, upper_second = -1;

    if(!cells.empty())
    {
        auto one = std::minmax_element(cells.cbegin(), cells.cend(), [](
                                       const Pente::field_type& c1, const Pente::field_type& c2){
            return c1.first < c2.first;
        });

        lower_first = one.first->first;
        upper_first = one.second->first;

        auto two = std::minmax_element(cells.cbegin(), cells.cend(), [](
                                       const Pente::field_type& c1, const Pente::field_type& c2){
            return c1.first < c2.first;
        });

        lower_second = two.first->second;
        upper_second = two.second->second;
    }
    else
    {
        lower_first = lower_second = upper_first = upper_second = BoardSize / 2;
    }

    return std::make_tuple(lower_first, upper_first, lower_second, upper_second);
}
