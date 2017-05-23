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
	char player = 'b', opponent = 'w', empty = 'o';
private:
	bool isMovesLeft(char board[N][N]);
	//int evaluate(char b[N][N]);
	int minimax(char board[N][N], int depth, bool isMax);
};

#endif // ALGORITHM
