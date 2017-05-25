#ifndef VALUE_H
#define VALUE_H

class Evaluation : public Algorithm
{
public:
	Evaluation(){}
	~Evaluation(){}	
	int evaluate(std::vector < char > board_vec);
};

#endif // VALUE
