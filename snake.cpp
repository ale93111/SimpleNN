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
	
	Generation X(4,{8,8},4);

	for(int i=0; i<population; i++)
		X.Trainees.push_back(std::make_shared<GameSession>());


	X.EvolveUntilFitnessEqual(3000.0);

	endwin();
	
	std::cout << "Training complete!" << std::endl;
	std::cout << "number of generations = " << X.numberofGenerations << std::endl;

	//GameSession GottaCatchEmAll(500);
	//GottaCatchEmAll.AI_play();

	Genome champ = X.genomes[0];

	champ.SavetoFile("champ.nn");
	//champ.network.print();

	

	return 0;
}