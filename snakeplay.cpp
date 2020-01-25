#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include <ncursesw/ncurses.h>
#include <locale.h>
#include <unistd.h> //sleep

//#include "ga.h"
#include "snake_ai/board.h"
#include "snake_ai/gamesession.h"


int main()
{
	// Initilize Random Generator
	srand((unsigned)time(0U));
	//srand (1);

	//initialize ncurses
	setlocale(LC_ALL,"");
	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	noecho();
	//resizeterm(12,12);
	//int err = system("resize -s 3 30");

	attron(COLOR_PAIR(1));
	
	Network n(24,{16},4);
	Genome champ(n,0);	

	

	champ.ReadfromFile("champ.nn");
	//champ.network.print();

	GameSession GottaCatchEmAll(100);
	GottaCatchEmAll.AI_play(champ);

	endwin();

	return 0;
}