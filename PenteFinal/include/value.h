#ifndef VALUE_H
#define VALUE_H

#include <vector>
#include <map>
#include <tuple>

#include "pente.h"

class PenteEvaluation
{
public:
    enum class PlayerType {
        PLAYER = 0,
        OPPONENT = 1
    };

    PenteEvaluation(Player::PlayerColours playing_player = Player::PlayerColours::WHITE,
                    float player_line_weigth = 1.0f, float opponent_line_weigth = 1.0f,
                    float player_pair_weigth = 1.0f, float opponent_pair_weigth = 1.0f);
    ~PenteEvaluation();
    float evaluate(const Pente& game_state);
    std::tuple<int, int, int, int> getAreaOfCare(const Pente& game_state);


private:
    // This function searches for all the length of lines of give colour in the board
    std::vector<int> checkForLinesByPlayerType(const Pente& game_state,
                                           const PlayerType player);

    // This function checks the length of a line starting at given cell
    int checkForLineAtCellWithDirection(const Pente& game_state,
                                        const Pente::field_type& cell,
                                        const Pente::field_type& direction,
                                        const PlayerType player);

    std::tuple<int, int, int, int> getBordersForSet(const std::set<Pente::field_type>& cells);

    const Player::PlayerColours PLAYING_PLAYER_;
    std::map<PlayerType, Player::PlayerColours> player_to_colour_;


    const float PLAYER_LINE_WEIGTH_;
    const float OPPONENT_LINE_WEIGTH_;

    const float PLAYER_PAIR_WEIGTH_;
    const float OPPONENT_PAIR_WEIGTH_;

    const int AreaOfCareExtend = 2;
};

#endif // VALUE
