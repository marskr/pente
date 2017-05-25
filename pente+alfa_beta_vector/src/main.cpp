#include <iostream>
#include <iomanip>
#include <assert.h>
#include <vector>
#include <cmath>
#include "../include/pente.h"
#include "../include/algorithm.h"
#include "../include/value.h"

const int M = 19;

std::vector < char > printBoard(Pente::board_type board, std::vector < char > board_vec)
{
    //int i = 0, j = 0;
	//std::vector < char > board;
    // auto -> variable declared will be automatically deducated from its initializer
    for(auto it = board.cbegin(); it != board.cend(); ++it)
    {
        for(auto in_it = it->cbegin(); in_it != it->cend(); ++in_it)
        {
            std::cout << std::setw(4);

            switch(*in_it)
            {
                case Player::PlayerColours::NONE: std::cout << "o"; 
					board_vec.push_back('o');
                    break;

                case Player::PlayerColours::BLACK: std::cout << "b"; 
					board_vec.push_back('b');
                    break;

                case Player::PlayerColours::WHITE: std::cout << "w"; 
 					board_vec.push_back('w');
                    break;

                default: assert(false);
            }
        }
		//j = 0;
    	std::cout << std::endl;
    }
    return board_vec;
}

int main()
{
    Pente pente;
    pente.initGame();

    bool Change = true;
    std::vector < char > board1;
	


    while(true)
    {
        board1 = printBoard(pente.getBoard(), board1);


        std::cout << "This is turn of player: " << pente.getCurrentPlayer() << std::endl;
        std::cout << "White player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::WHITE) << " pairs." << std::endl;
        std::cout << "Black player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::BLACK) << " pairs." << std::endl;
	        
	int row, column;
	
	//std::cout << "CURRENT PLAYER: " <<pente.getCurrentPlayer();
	
	if (Change==true)
	{
        std::cout << "What row?" << std::endl;
        std::cin >> row;

        std::cout << "What column?" << std::endl;
        std::cin >> column;
	else
	{
	    	Algorithm algo1;
	 	int bestMove = algo1.findBestMove(board1);
		std::cout << "\n\n BEST MOVE IS: " << bestMove << '\n';
		row = bestMove/BoardSize;
		column = bestMove%BoardSize;
		//std::cout << " ROW: " << row << '\n';
		//std::cout << " COLUMN: " << column << '\n';
	}
    pente.markCell(row,column);

	Change=!Change;
    if(pente.getIsWon())
    	std::cout << pente.getCurrentPlayer() << " player won the game!!!" << std::endl;
    }
	
    return 0;
}
