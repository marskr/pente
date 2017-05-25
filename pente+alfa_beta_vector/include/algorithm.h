#ifndef ALGORITHM_H
#define ALGORITHM_H	

const int N = 19;

struct Move
{
	int row, col;
};

class Algorithm
{
public:
	Algorithm(){}
	~Algorithm(){}
	int findBestMove(std::vector < char > board_vec);
	char get_player(){return player;}
	void set_player(char player){this->player = player;}
	char get_opponent(){return opponent;}
	void set_opponent(char opponent){this->opponent = opponent;}
	char get_empty(){return empty;}
	void set_empty(char empty){this->empty = empty;}
private:
	bool isMovesLeft(std::vector < char > board_vec);
	//int evaluate(char b[N][N]);
	int minimax(std::vector < char > board_vec, int depth, bool isMax);
	//int abminimax(char board[N][N], int depth, bool isMax);	
	//int alfabeta(char board[N][N], int depth, bool isMax, int alfa, int beta);
	char player = 'b', opponent = 'w', empty = 'o';
};

#endif // ALGORITHM
