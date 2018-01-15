#include <unistd.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>

#include "../include/pente.h"
#include "../include/algorithm.h"
#include "../include/value.h"
#include "../include/timer.h"

#include <mpi.h>

const float PLAYERLINEWEIGHT = 1.0f;
const float OPPONENTLINEWEIGHT = 1.0f;
const float PLAYERPAIRWEIGHT = 20.0f;
const float OPPONENTPAIRWEIGHT = 2.0f;

const double TIME_CONTROLLER = std::numeric_limits<double>::infinity();
int DEPTH_CONTROLLER = 1;
int NUMBER_OF_THREADS;

/* FOR SECOND IA */
const float PLAYERLINEWEIGHT_2 = 4.0f;
const float OPPONENTLINEWEIGHT_2 = 2.0f;
const float PLAYERPAIRWEIGHT_2 = 1.0f;
const float OPPONENTPAIRWEIGHT_2 = 1.0f;

const int DATA_SIZE = 10;

int main(int argc, char **argv) {

    // ------------------------------
    //// Pente game init

    std::stringstream ss;
    {
        Pente pente;
        pente.initGame();

        pente.markCell({0, 0});
        pente.markCell({1, 1});

        std::cout << "Before serialization:" << std::endl;

        for(auto& row : pente.getBoard())
        {
            for(auto& el : row)
            {
                switch(el)
                {
                case Player::PlayerColours::NONE: std::cout << "-";
                    break;

                case Player::PlayerColours::WHITE: std::cout << "W";
                    break;

                case Player::PlayerColours::BLACK: std::cout << "B";
                    break;

                default: throw std::invalid_argument("WTF?!");
                }

                std::cout << " ";
            }

            std::cout << std::endl;
        }

        std::cout << "Saving to stream." << std::endl;
        pente.saveToStream(ss);
    }


    Pente pente2;
    std::cout << "Loading from stream." << std::endl;
    pente2.loadFromStream(ss);

    std::cout << "After deserialization:" << std::endl;

    for(auto& row : pente2.getBoard())
    {
        for(auto& el : row)
        {
            switch(el)
            {
            case Player::PlayerColours::NONE: std::cout << "-";
                break;

            case Player::PlayerColours::WHITE: std::cout << "W";
                break;

            case Player::PlayerColours::BLACK: std::cout << "B";
                break;

            default: throw std::invalid_argument("WTF?!");
            }

            std::cout << " ";
        }

        std::cout << std::endl;
    }



//    std::chrono::time_point <std::chrono::system_clock> start, stop;

//    PenteEvaluation white_eval(Player::PlayerColours::WHITE,
//                               PLAYERLINEWEIGHT_2, OPPONENTLINEWEIGHT_2,
//                               PLAYERPAIRWEIGHT_2, OPPONENTPAIRWEIGHT_2);

//    PenteEvaluation black_eval(Player::PlayerColours::BLACK,
//                               PLAYERLINEWEIGHT, OPPONENTLINEWEIGHT,
//                               PLAYERPAIRWEIGHT, OPPONENTPAIRWEIGHT);

//    Algorithm search;

//    if(pente.getCurrentPlayer() == Player::PlayerColours::WHITE)
//    {
//        Move bestMove2 = search.findBestMove(pente, white_eval, DEPTH_CONTROLLER, TIME_CONTROLLER, Algorithm::SearchType::PARALLEL_MINMAX_SHARED);
//    }
//    else
//    {
//        Move bestMove2 = search.findBestMove(pente, black_eval, DEPTH_CONTROLLER, TIME_CONTROLLER, Algorithm::SearchType::PARALLEL_MINMAX_SHARED);
//    }

    return 0;
}
