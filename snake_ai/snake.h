#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "board.h"

struct Snake
{
	std::vector<int> y, x;
	std::vector<Direction> direction, olddirection;
	int size;
	int score;
	//bool cangrow;
	
	void display()
	{
		for(int i=0; i<size; i++) mvprintw(y[i],x[i],"@");
	}
	
	bool check_coord(Board & terminal)
	{
		//check boundaries
		if(x.front() < 0) {x.front() = 0; return true;}
		else if(x.front() >= terminal.xmax) {x.front() = terminal.xmax - 1; return true;}

		if(y.front() < 0) {y.front() = 0; return true;}
		else if(y.front() >= terminal.ymax) {y.front() = terminal.ymax - 1; return true;}
		
		//check body
		for(int i=1; i<size; i++)
		{
			if( x.front() == x.at(i) &&  y.front() == y.at(i) ) return true;
		}
		
		return false;
	}
	
	void move(int c)
	{
		olddirection.front() = direction.front();
		
		for(int i=size-1; i>0; i--)  direction.at(i) = direction.at(i-1);
		
		//get new direction
		switch(c)
		{
			case KEY_UP:    direction.front() = UP; 
				break;
			case KEY_DOWN:  direction.front() = DOWN;
				break;
			case KEY_RIGHT: direction.front() = RIGHT;
				break;
			case KEY_LEFT:  direction.front() = LEFT;
				break;
			//default:		direction = NONE;
			//	break;				
		}
		
		//can't move backwards
		//if( (olddirection.front() == UP    && direction.front() == DOWN) || (olddirection.front() == DOWN && direction.front() == UP) ||
		//    (olddirection.front() == RIGHT && direction.front() == LEFT) || (olddirection.front() == LEFT && direction.front() == RIGHT)) direction.front() = olddirection.front();
		
		//move
		if(direction.front() == UP)    y.front()--;
		else if(direction.front() == DOWN)  y.front()++;
		else if(direction.front() == LEFT)  x.front()--;
		//else if(direction == RIGHT) x.front()++;
		else x.front()++;
			
		//update body directions
		for(int i=1; i<size; i++)
		{
			if(direction.at(i-1) == UP)    
			{
				y.at(i) = y.at(i-1) + 1;
				x.at(i) = x.at(i-1);
			}
			else if(direction.at(i-1) == DOWN)  
			{
				y.at(i) = y.at(i-1) - 1;
				x.at(i) = x.at(i-1);
			}
			else if(direction.at(i-1) == LEFT)  
			{
				y.at(i) = y.at(i-1);
				x.at(i) = x.at(i-1) + 1;
			}
			else 
			{
				y.at(i) = y.at(i-1);
				x.at(i) = x.at(i-1) - 1;
			}
		}
		
	}
	
	void grow()
	{
		size++;
		
		y.push_back(y.back());
		x.push_back(x.back());
		direction.push_back(direction.back());
	}

	void lunch()
	{
		score += 1000;
		//if(cangrow) grow();
	}
	

	void allocator( int N )
	{
		y.resize(N);
		x.resize(N);
		direction.resize(N);
		olddirection.resize(N);
	}


	void reset(int ystart, int xstart, Direction directioni)
	{
		score = 0;
		size = 1;

		allocator(0);

		y.push_back(ystart);
		x.push_back(xstart);
		direction.push_back(directioni);
		olddirection.push_back(directioni);
	}
	
	Snake(){}
	Snake(int yi, int xi, Direction directioni) : score(0), size(1) 
	{
		y.push_back(yi);
		x.push_back(xi);
		direction.push_back(directioni);
		olddirection.push_back(directioni);

		//starting body
		//if(cangrow) {grow();grow();grow();}
	}
	
	~Snake(){}

	Snake& operator=(Snake&& other)
	{
		size = other.size;
		score = other.score;

		allocator(size);
		
		for(int i=0; i<size; i++) 
		{
			y[i]            = other.y[i];
			x[i]            = other.x[i];
			direction[i]    = other.direction[i];
			olddirection[i] = other.olddirection[i];
		}
		
		return *this;
	}
};

#endif
