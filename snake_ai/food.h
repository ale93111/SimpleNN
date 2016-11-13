#ifndef FOOD_H
#define FOOD_H

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "board.h"
#include "snake.h"

struct Food
{
	int y, x;
	bool IsThereFood;
	
	void display()
	{
		if(IsThereFood) mvprintw(y,x,"•");
	}
	
	void GenerateOn(Board & terminal)
	{
		y = rand() % terminal.ymax;
		x = rand() % terminal.xmax;
	}
	
	bool EatenBy(Snake & Ekans)
	{
		if( Ekans.y.front() == y && Ekans.x.front() == x) 
		{
			Ekans.lunch();
			return true;
		}
		else return false;
	}

	void check(Snake & Ekans, Board & terminal)
	{
		if(IsThereFood)
		{
			//IsThereFood = EatenBy(Ekans)? false : true;
			if(EatenBy(Ekans)) IsThereFood = false;
			else IsThereFood = true;
		}
		else
		{
			GenerateOn(terminal);
			IsThereFood = true;
		}

		//display();
		
	}
		
	Food(){}
	Food(Board & terminal) : IsThereFood(true) 
	{
		GenerateOn(terminal);
	}
	~Food(){}
};

#endif