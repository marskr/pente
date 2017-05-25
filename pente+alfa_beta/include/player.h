#ifndef PLAYER_H
#define PLAYER_H

#include <stdexcept>

class Player
{
public:

    enum class PlayerColours {
        NONE = 0,
        WHITE  = 1,
        BLACK = 2
    };

    Player();
    ~Player();
    // Increment the number of opponent's pairs beaten by the player
    void incrementCapturedPairs();
    void setCapturedPairs(int captured_pairs);
    int getCapturedPairs() const;

private:
    static int number_of_players_; // The field used globally for every user
    int captured_pairs_;

    PlayerColours colour_;
};

#endif // PLAYER_H
