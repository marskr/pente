#include <iostream>
#include <iomanip>
#include <assert.h>
#include "../include/pente.h"

void printBoard(Pente::board_type board)
{
    for(auto it = board.cbegin(); it != board.cend(); ++it)
    {
        for(auto in_it = it->cbegin(); in_it != it->cend(); ++in_it)
        {
            std::cout << std::setw(4);

            switch(*in_it)
            {
                case Player::PlayerColours::NONE: std::cout << "o";
                    break;

                case Player::PlayerColours::BLACK: std::cout << "b";
                    break;

                case Player::PlayerColours::WHITE: std::cout << "w";
                    break;

                default: assert(false);
            }
        }

        std::cout << std::endl;
    }
}

int main()
{
    Pente pente;
    pente.initGame();

    while(true)
    {
        printBoard(pente.getBoard());

        std::cout << "This is turn of player: " << pente.getCurrentPlayer() << std::endl;
        std::cout << "White player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::WHITE) << " pairs." << std::endl;
        std::cout << "Black player beat " << pente.getCapturedPairsOfPlayer(Player::PlayerColours::BLACK) << " pairs." << std::endl;
	        
	int row, column;

        std::cout << "What row?" << std::endl;
        std::cin >> row;

        std::cout << "What column?" << std::endl;
        std::cin >> column;

	

        pente.markCell(row, column);

        if(pente.getIsWon())
            std::cout << pente.getCurrentPlayer() << " player won the game!!!" << std::endl;
    }
	
    return 0;
}
