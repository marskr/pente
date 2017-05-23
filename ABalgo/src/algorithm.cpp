#include <iostream>
#include <bits/stdc++.h>

#include "../include/algorithm.h"
#include "../include/value.h"

using namespace std;

// C++ program to find the next optimal move for
// a player

// This function returns true if there are moves
// remaining on the board. It returns false if
// there are no moves left to play.
bool Algorithm::isMovesLeft(char board[N][N])
{
	for (int i = 0; i<N; i++)
		for (int j = 0; j<N; j++)
			if (board[i][j]==Algorithm::empty)
				return true;
	return false;
}

// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
// https://pl.wikipedia.org/wiki/Algorytm_alfa-beta
// https://github.com/mcostalba/Stockfish/blob/master/src/search.cpp
int Algorithm::minimax(char board[N][N], int depth, bool isMax)
{
	Evaluation eval1;
	int score = eval1.evaluate(board);

	// If Maximizer has won the game return his/her
	// evaluated score
	if (score == 10)
		return score;

	// If Minimizer has won the game return his/her
	// evaluated score
	if (score == -10)
		return score;

	// If there are no more moves and no winner then
	// it is a tie
	if (Algorithm::isMovesLeft(board)==false)
		return 0;

	// If this maximizer's move
	if (isMax)
	{
		int best = -1000;

		// Traverse all cells
		for (int i = 0; i<N; i++)
		{
			for (int j = 0; j<N; j++)
			{
				// Check if cell is empty
				if (board[i][j]==Algorithm::empty)
				{
					// Make the move
					board[i][j] = Algorithm::player;

					// Call minimax recursively and choose
					// the maximum value
					//////////////////////std::cout << "\n DEPTH: " << depth << '\n';
					if (depth >= 20)
						return best;
					best = max(best,Algorithm::minimax(board, depth+1, !isMax));

					// Undo the move
					board[i][j] = Algorithm::empty;
				}
			}
		}
		return best;
	}

	// If this minimizer's move
	else
	{
		int best = 1000;

		// Traverse all cells
		for (int i = 0; i<N; i++)
		{
			for (int j = 0; j<N; j++)
			{
				// Check if cell is empty
				if (board[i][j]==Algorithm::empty)
				{
					// Make the move
					board[i][j] = Algorithm::opponent;

					// Call minimax recursively and choose
					// the minimum value
					best = min(best,Algorithm::minimax(board, depth+1, !isMax));

					// Undo the move
					board[i][j] = Algorithm::empty;
				}
			}
		}
		return best;
	}
}

// This will return the best possible move for the player
Move Algorithm::findBestMove(char board[N][N])
{
	int bestVal = -1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;

	// Traverse all cells, evalutae minimax function for
	// all empty cells. And return the cell with optimal
	// value.
	for (int i = 0; i<N; i++)
	{
		for (int j = 0; j<N; j++)
		{
			// Check if celll is empty
			if (board[i][j]==Algorithm::empty)
			{
				// Make the move
				board[i][j] = Algorithm::player;

				// compute evaluation function for this
				// move.
				int moveVal = Algorithm::minimax(board, 0, false);

				// Undo the move
				board[i][j] = Algorithm::empty;

				// If the value of the current move is
				// more than the best value, then update
				// best/
				if (moveVal > bestVal)
				{
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}

	printf("The value of the best Move is : %d\n\n",bestVal);

	return bestMove;
}
