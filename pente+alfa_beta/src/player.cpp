#include "../include/player.h"

Player::Player()
{
    number_of_players_++;

    // The first player is assigned white colour, second black. More are not supported.
    switch(number_of_players_)
    {
        case 1: colour_ = PlayerColours::WHITE; // The first player is assigned to white colour
            break;

        case 2: colour_ = PlayerColours::BLACK; // The secound player is assigned to black colour
            break;

        default: throw std::runtime_error("There can be no more than 2 player and not less than 1 !!!");
    }
}

Player::~Player()
{
    number_of_players_--;
}

void Player::incrementCapturedPairs()
{
    captured_pairs_++; // Here's amount of pairs, which were 
}

void Player::setCapturedPairs(int captured_pairs)
{
    captured_pairs_ = captured_pairs;
}

int Player::getCapturedPairs() const
{
    return captured_pairs_;
}

int Player::number_of_players_ = 0;
