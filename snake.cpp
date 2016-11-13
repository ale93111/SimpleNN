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
	GameSession GottaCatchEmAll(500);

	//GottaCatchEmAll.play();

	endwin();


	/*
	Generation X(2,{3},1);

	for(int i=0; i<population; i++)
		X.Trainees.push_back(std::make_shared<XORSolver>());

	//X.Trainees = Trainees;

	X.EvolveUntilFitnessEqual(16.0);
	
	std::cout << "XOR solved!!" << std::endl;
	std::cout << "number of generations = " << X.numberofGenerations << std::endl;

	Genome champ = X.genomes[0];
	//revaluate champ
	std::vector<std::vector<double>> dataset = {{1,0},{1,1},{0,1},{0,0}};
	std::vector<double> output;
	for(auto input : dataset)
	{
		auto c = champ.network.FeedForward(input);
		output.push_back(c[0]);
	}

	for(int i=0; i<output.size(); i++)
	{
		std::cout<< "the output of {" << dataset[i][0] << "," << dataset[i][1] << "} is " << output[i] << " ( = " << int(output[i]>0.5) << " )" << std::endl;		
	}

	champ.SavetoFile("champ.nn");
	champ.network.print();
*/
	return 0;
}