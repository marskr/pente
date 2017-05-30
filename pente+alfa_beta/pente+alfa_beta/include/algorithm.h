#ifndef ALGORITHM_H
#define ALGORITHM_H	

const int N = 19;

struct Move
{
	int row, col;
};

//BASIC CLASS FOR "EVALUATION"
class Algorithm
{
public:
	Algorithm(){}
	~Algorithm(){}
	Move findBestMove(char board[N][N]);
	char get_player(){return player;}
	void set_player(char player){this->player = player;}
	char get_opponent(){return opponent;}
	void set_opponent(char opponent){this->opponent = opponent;}
	char get_empty(){return empty;}
	void set_empty(char empty){this->empty = empty;}
private:
	bool isMovesLeft(char board[N][N]);
	//int evaluate(char b[N][N]);
	int minimax(char board[N][N], int depth, bool isMax);
	int abminimax(char board[N][N], int depth, bool isMax);	
	int alfabeta(char board[N][N], int depth, bool isMax, int alfa, int beta);
	char player = 'b', opponent = 'w', empty = 'o';
};

#endif // ALGORITHM
