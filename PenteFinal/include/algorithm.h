#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <tuple>
#include <chrono>

#include "value.h"

struct Move
{
	int row, col;
};

//BASIC CLASS FOR "EVALUATION"
class Algorithm
{
public:
    Algorithm(){}
    ~Algorithm(){}
    Move findBestMove(Pente& pente, PenteEvaluation& evaluation,
                      int depth, double time_seconds);

private:
    bool isMovesLeft(Pente& pente);
    std::pair<float, Move> abminimax(Pente& pente, PenteEvaluation& evaluation,
                                     int depth, bool isMax, Move move);
    std::pair<float, Move> alfabeta(Pente& pente, PenteEvaluation& evaluation,
                                    int depth, bool isMax, float alfa, float beta, Move move);

    std::chrono::time_point<std::chrono::system_clock> time_start_;
    std::chrono::duration<double> time_seconds_;
};

#endif // ALGORITHM
