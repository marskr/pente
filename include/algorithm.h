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
	enum class SearchType {
		MINMAX = 0,
		ALFABETA = 1
	};
    Algorithm(){}
    ~Algorithm(){}
    Move findBestMove(Pente& pente, PenteEvaluation& evaluation,
                      int depth, double time_seconds, SearchType type = SearchType::MINMAX);

private:
    bool isMovesLeft(Pente& pente);
    std::pair<float, Move> abminimax(Pente& pente, PenteEvaluation& evaluation,
                                     int depth, bool isMax, Move move);
	std::pair<float, Move> MinMaxSearch(Pente& pente, PenteEvaluation& evaluation,
									int depth, bool is_max, Move move);
    std::pair<float, Move> AlfaBetaSearch(Pente& pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, float alfa, float beta, Move move);

    std::chrono::time_point<std::chrono::system_clock> time_start_;
    std::chrono::duration<double> time_seconds_;
};

#endif // ALGORITHM
