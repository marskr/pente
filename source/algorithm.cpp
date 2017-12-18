#include <iostream>
#include <cmath>
#include <limits>

#include <omp.h>

#include "../include/algorithm.h"

// C++ program to find the next optimal move for
// a player

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool Algorithm::isMovesLeft(Pente &pente)
{
	return (pente.getWhiteCells().size() + pente.getBlackCells().size() < BoardSize*BoardSize);
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
// https://pl.wikipedia.org/wiki/Algorytm_alpha-beta
// https://github.com/mcostalba/Stockfish/blob/master/src/search.cpp
std::pair<float, Move> Algorithm::minMaxSearch(Pente& pente, PenteEvaluation& evaluation,
                                               int depth, bool is_max, Move move)
{
    ++last_nodes_explored_;

    if (pente.getIsWon() ||
        depth == 0)
    {
        float score = evaluation.evaluate(pente);
        return std::make_pair(score, move);
    }

    // If there are no more moves and no winner then
    // it is a tie
    if (!isMovesLeft(pente))
        return std::make_pair(0, move);

    int one_low, one_high, two_low, two_high;
    std::tie(one_low, one_high, two_low, two_high) = PenteEvaluation().getAreaOfCare(pente);

    double best_score;
    if(is_max)
    {
        best_score = -std::numeric_limits<double>::infinity();
    }
    else
    {
        best_score = std::numeric_limits<double>::infinity();
    }

    // Traverse all cells
    for (int i = one_low; i < one_high; i++)
    {
        for (int j = two_low; j < two_high; j++)
        {
            // Check if cell is empty
            if (isMovesLeft(pente))
            {
                if(pente.getCellValue(std::make_pair(i, j)) != Player::PlayerColours::NONE)
                    continue;
                // Make the move
                pente.markCell(std::make_pair(i,j));

                double score = minMaxSearch(pente, evaluation, depth - 1, !is_max, move).first;

                if(is_max)
                {
                    if(score > best_score)
                    {
                        move.row = i;
                        move.col = j;

                        best_score = score;
                    }
                }
                else
                {
                    if(score < best_score)
                    {
                        move.row = i;
                        move.col = j;

                        best_score = score;
                    }
                }

                pente.undoLastMove();
            }
        }
    }
    return std::make_pair(best_score, move);
}

std::pair<float, Move> Algorithm::parallelMinMaxSearch(Pente pente, PenteEvaluation& evaluation,
                                                  int depth, bool is_max, Move move)
{
    if (pente.getIsWon() ||
        depth == 0)
    {
        float score = evaluation.evaluate(pente);
        return std::make_pair(score, move);
    }

    // If there are no more moves and no winner then
    // it is a tie
    if (!isMovesLeft(pente))
        return std::make_pair(0, move);

    int one_low, one_high, two_low, two_high;
    std::tie(one_low, one_high, two_low, two_high) = PenteEvaluation().getAreaOfCare(pente);

    double best_score;
    if(is_max)
    {
        best_score = -std::numeric_limits<double>::infinity();
    }
    else
    {
        best_score = std::numeric_limits<double>::infinity();
    }

    // Traverse all cells
    #pragma omp parallel for ordered collapse(2) firstprivate(pente)
    for (int i = one_low; i < one_high; i++)
    {
        for (int j = two_low; j < two_high; j++)
        {
            // Check if cell is empty
            if (isMovesLeft(pente) &&
                    pente.getCellValue(std::make_pair(i, j)) == Player::PlayerColours::NONE)
            {
                // Make the move
                pente.markCell(std::make_pair(i,j));


                double score = minMaxSearch(pente, evaluation, depth - 1, !is_max, move).first;

                #pragma omp critical
                {
                    if(is_max)
                    {
                        if(score > best_score)
                        {
                            move.row = i;
                            move.col = j;

                            best_score = score;
                        }
                    }
                    else
                    {
                        if(score < best_score)
                        {
                            move.row = i;
                            move.col = j;

                            best_score = score;
                        }
                    }
                }

                pente.undoLastMove();
            }
        }
    }
    return std::make_pair(best_score, move);

}

std::pair<float, Move> Algorithm::alphaBetaSearch(Pente& pente, PenteEvaluation& evaluation,
                                                 int depth, bool is_max, float alpha, float beta, Move move)
{
    ++last_nodes_explored_;

    if (pente.getIsWon() ||
        depth == 0)
    {
        float score = evaluation.evaluate(pente);
        return std::make_pair(score, move);
    }

	// If there are no more moves and no winner then
	// it is a tie
    if (!isMovesLeft(pente))
        return std::make_pair(0, move);

    int one_low, one_high, two_low, two_high;
    std::tie(one_low, one_high, two_low, two_high) = PenteEvaluation().getAreaOfCare(pente);

    double best_score;
    if(is_max)
    {
        best_score = -std::numeric_limits<double>::infinity();
    }
    else
    {
        best_score = std::numeric_limits<double>::infinity();
    }

    // Traverse all cells
    for (int i = one_low; i < one_high; i++)
    {
        for (int j = two_low; j < two_high; j++)
        {
            // Check if cell is empty
            if (isMovesLeft(pente))
            {
                if(pente.getCellValue(std::make_pair(i, j)) != Player::PlayerColours::NONE)
                    continue;
                // Make the move
                pente.markCell(std::make_pair(i,j));

                double score = alphaBetaSearch(pente, evaluation, depth - 1, !is_max, alpha, beta, move).first;
                if(is_max)
                {
                    if(score > alpha)
                    {
                        move.row = i;
                        move.col = j;

                        alpha = score;
                    }
                }
                else
                {
                    if(score < beta)
                    {
                        move.row = i;
                        move.col = j;

                        beta = score;
                    }
                }

                pente.undoLastMove();

                if (alpha >= beta)
                {
                    if(is_max)
                    {
                        best_score = alpha; // beta cutoff
                    }
                    else
                    {
                        best_score = beta; // alpha cutoff
                    }
                    return std::make_pair(best_score, move);
                }
            }
        }
    }

    if(is_max)
    {
        best_score = alpha;
    }
    else
    {
        best_score = beta;
    }
    return std::make_pair(best_score, move);
}

// This will return the best possible move for the player
Move Algorithm::findBestMove(Pente &pente, PenteEvaluation& evaluation,
                             int depth, double time_seconds, SearchType type)
{
    last_nodes_explored_ = 0;

	float best_val = -std::numeric_limits<float>::infinity();
    Move best_move = {0, 0};

    double start_time = omp_get_wtime();

    switch(type)
    {
        case SearchType::MINMAX:
            std::tie(best_val, best_move) = minMaxSearch(pente, evaluation, depth, true, best_move);
            break;

        case SearchType::PARALLEL_MINMAX:
            std::tie(best_val, best_move) = parallelMinMaxSearch(pente, evaluation, depth, true, best_move);
            break;

        case SearchType::ALPHABETA:
            std::tie(best_val, best_move) = alphaBetaSearch(pente, evaluation, depth, true,
                                                           -std::numeric_limits<float>::infinity(),
                                                           std::numeric_limits<float>::infinity(), best_move);
            break;
    }

    double end_time = omp_get_wtime();
    last_move_time_ = end_time - start_time;

	return best_move;
}
