#include <iostream>
#include <bits/stdc++.h>

#include "../include/algorithm.h"
#include "../include/value.h"

int Evaluation::evaluate(char b[N][N])
{
	// Checking for Rows for X or O victory.
	for (int row = 0; row<N; row++)
	{
		for(int j = 0; j<N; j++)
		{
			if (b[row][j]==b[row][j+1] && b[row][j+1]==b[row][j+2] && b[row][j+2]==b[row][j+3])
			{
				if (b[row][j]==Evaluation::get_player())
					return +10;
				else if (b[row][j]==Evaluation::get_opponent())
					return -10;
			}
		}
	}

	// Checking for Columns for X or O victory.
	for (int col = 0; col<N; col++)
	{
		for(int i = 0; i<N; i++)
		{
			if (b[i][col]==b[i+1][col] && b[i+1][col]==b[i+2][col] && b[i+2][col]==b[i+3][col])
			{
				if (b[i][col]==Evaluation::get_player())
					return +10;

				else if (b[i][col]==Evaluation::get_opponent())
					return -10;
			}
		}
	}

	// Checking for Diagonals for X or O victory.
	for(int i = 0; i<N;i++)
	{
		for(int j = 0; j<N;j++)
		{
			if (i==j)
			{
				if (b[i][j]==b[i+1][j+1] && b[i+1][j+1]==b[i+2][j+2] && b[i+2][j+2]==b[i+3][j+3])
				{
					if (b[i][j]==Evaluation::get_player())
						return +10;
					else if (b[i][j]==Evaluation::get_opponent())
						return -10;
				}
			}
		}
	}
	for(int i = 0; i<N;i++)
	{
		for(int j = 0; j<N;j++)
		{
			if (i==N-j-1)
			{
				if (b[i][N-j-1]==b[i-1][N-j] && b[i-1][N-j]==b[i-2][N-j+1] && b[i-2][N-j+1]==b[i-3][N-j+2])
				{
					if (b[i][N-j-1]==Evaluation::get_player())
						return +10;
					else if (b[i][N-j-1]==Evaluation::get_opponent())
						return -10;
				}
			}
		}
	}

	// Else if none of them have won then return 0
	return 0;
}

