#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <tuple>
#include <chrono>
#include <iostream>

#include <omp.h>

#include "value.h"

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
        PARALLEL_MINMAX = 1,
		ALPHABETA = 2
	};

    Algorithm()
    {
        omp_set_nested(0);
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

    std::pair<float, Move> parallelMinMaxSearch(Pente pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, Move move);

    std::pair<float, Move> alphaBetaSearch(Pente& pente, PenteEvaluation& evaluation,
                                    int depth, bool is_max, float alpha, float beta, Move move);

    double last_move_time_;
	long last_nodes_explored_;
};

#endif // ALGORITHM
