#include <iostream>
#include <iomanip>
#include <assert.h>
#include "../include/pente.h"
#include "../include/algorithm.h"
#include "../include/value.h"

const int M = 19;

char printBoard(Pente::board_type board, char Board[M][M])
{
    int i = 0, j = 0;
    for(auto it = board.cbegin(); it != board.cend(); ++it, ++i)
    {
        for(auto in_it = it->cbegin(); in_it != it->cend(); ++in_it, ++j)
        {
            std::cout << std::setw(4);

            switch(*in_it)
            {
                case Player::PlayerColours::NONE: std::cout << "o"; 
						  Board[i][j] = 'o';
                    break;

                case Player::PlayerColours::BLACK: std::cout << "b"; 
						   Board[i][j] = 'b';
                    break;

                case Player::PlayerColours::WHITE: std::cout << "w"; 
 						   Board[i][j] = 'w';
                    break;

                default: assert(false);
            }
        }
	j = 0;
        std::cout << std::endl;
    }
    return Board[M][M];
}

int main()
{
    Pente pente;
    pente.initGame();

    bool Change = true;
    char board1[M][M];

    while(true)
    {
        board1[M][M] = printBoard(pente.getBoard(), board1);

        std::cout << "This is turn of player: " << pente.getCurrentPlayer() << std::endl;
        std::cout << "White player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::WHITE) << " pairs." << std::endl;
        std::cout << "Black player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::BLACK) << " pairs." << std::endl;
	        
	int row, column;

	if (Change)
	{
            std::cout << "What row?" << std::endl;
            std::cin >> row;

            std::cout << "What column?" << std::endl;
            std::cin >> column;
	}
	else
	{
	    Algorithm algo1;
	    Move bestMove = algo1.findBestMove(board1);
	    row = bestMove.row;
	    column = bestMove.col;
	}
        pente.markCell(row,column);

	Change = !Change;
        if(pente.getIsWon())
            std::cout << pente.getCurrentPlayer() << " player won the game!!!" << std::endl;
    }
	
    return 0;
}
