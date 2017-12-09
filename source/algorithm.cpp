#include <iostream>
#include <cmath>
#include <limits>

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
// https://pl.wikipedia.org/wiki/Algorytm_alfa-beta
// https://github.com/mcostalba/Stockfish/blob/master/src/search.cpp
std::pair<float, Move> Algorithm::MinMaxSearch(Pente &pente, PenteEvaluation &evaluation,
                                               int depth, bool is_max, Move move)
{
    return std::make_pair(0.0, move);

}

std::pair<float, Move> Algorithm::AlfaBetaSearch(Pente &pente, PenteEvaluation& evaluation,
                                                 int depth, bool is_max, float alfa, float beta, Move move)
{
    //Move move;

    if (pente.getIsWon() ||
        depth == 0 ||
        (std::chrono::system_clock::now() - time_start_) > time_seconds_)
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

	// If this maximizer's move (turn of player)
	if (is_max)
	{
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

                    double potential_alfa = AlfaBetaSearch(pente, evaluation, depth-1, !is_max, alfa, beta, move).first;
                    if(potential_alfa > alfa)
                    {
                        move.row = i;
                        move.col = j;

                        alfa = potential_alfa;
                    }

                    pente.undoLastMove();

                    if (alfa>=beta)
                    {
                        return std::make_pair(alfa, move); // beta cutoff
                    }
				}
			}
		}
        return std::make_pair(alfa, move);
	}

	// If this minimizer's move (turn of opponent)
	else
	{
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

                    double potential_beta = AlfaBetaSearch(pente, evaluation, depth-1, !is_max, alfa, beta, move).first;
                    if(potential_beta < beta)
                    {
                        move.row = i;
                        move.col = j;

                        beta = potential_beta;
                    }

                    pente.undoLastMove();

                    if (alfa >= beta)
                    {
                        return std::make_pair(beta, move); // alfa cutoff
                    }
				}
			}
		}
        return std::make_pair(beta, move);
	}
}

// This will return the best possible move for the player
Move Algorithm::findBestMove(Pente &pente, PenteEvaluation& evaluation,
                             int depth, double time_seconds, SearchType type)
{
    time_start_ = std::chrono::system_clock::now();
    time_seconds_ = std::chrono::duration<double>(time_seconds);

	float best_val = -std::numeric_limits<float>::infinity();
    Move best_move = {0, 0};

    switch(type)
    {
        case SearchType::MINMAX:
            std::tie(best_val, best_move) = MinMaxSearch(pente, evaluation, depth, true, best_move);
            break;

        case SearchType ::ALFABETA:
            std::tie(best_val, best_move) = AlfaBetaSearch(pente, evaluation, depth, true,
                                                           -std::numeric_limits<float>::infinity(),
                                                           std::numeric_limits<float>::infinity(), best_move);
            break;
    }

	return best_move;
}
