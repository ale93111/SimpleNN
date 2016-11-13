#ifndef BOARD_H
#define BOARD_H

enum Direction {NONE, UP, DOWN, LEFT, RIGHT };

struct Board
{
	int ymax, xmax;
	
	Board(){}
	Board(int ymaxi, int xmaxi) : ymax(ymaxi), xmax(xmaxi) {}
	~Board(){}
};

#endif