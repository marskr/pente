#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <tuple>
#include <chrono>
#include <iostream>

#include <omp.h>

#include "value.h"

extern int NUMBER_OF_THREADS;

struct Move
{
	int row, col;

    bool operator != (const Move& rhs)
    {
        return (row != rhs.row) || (col != rhs.col);
    }
};

inline std::ostream& operator << (std::ostream& lhs, const Move& rhs)
{
    lhs << "Row: " << rhs.row << " Col: " << rhs.col;

    return lhs;
}

//BASIC CLASS FOR "EVALUATION"
class Algorithm
{
public:
	enum class SearchType {
            MINMAX = 0,
            PARALLEL_MINMAX_LOCAL = 1,
            PARALLEL_MINMAX_SHARED = 2,
            ALPHABETA = 3
	};

    Algorithm()
    {
        omp_set_nested(0);
        omp_set_dynamic(0);
    }
    ~Algorithm(){}
    Move findBestMove(Pente& pente, PenteEvaluation& evaluation,
                      int depth, double time_seconds, SearchType type = SearchType::MINMAX);

    double getLastMoveTime() const
    {
        return last_move_time_;
    }

    long getLastNodesExplored() const
    {
        return last_nodes_explored_;
    }

private:
    bool isMovesLeft(Pente& pente);

    std::pair<float, Move> minMaxSearch(Pente& pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, Move move);

    std::pair<float, Move> parallelMinMaxLocalSearch(Pente pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, Move move);

    std::pair<float, Move> parallelMinMaxSharedSearch(Pente pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, Move move);

    std::pair<float, Move> alphaBetaSearch(Pente& pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, float alpha, float beta, Move move);

    double last_move_time_;
	long last_nodes_explored_;
};

#endif // ALGORITHM
