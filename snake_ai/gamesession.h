#pragma once
#include <iostream>     // std::cout
#include <algorithm>    // std::max
#include <stdlib.h> 
#include <math.h>

#include "../ga.h"

#include "board.h"
#include "snake.h"
#include "food.h"

class GameSession : public ToTrain 
{
	public:
		Board terminal;
		Snake Ekans;
		Food pidgey; //or rattata

		double minDistanceFromFood;
		//snake starting point
		int xstart, ystart;
		int gamespeed; // bigger = slower (gamespeed->delay in millisecond)

		bool gameover;
		int command;

		virtual void Reset() override;
		virtual void Update(const std::vector<double>& networkOutputs) override;
		virtual double GetFitness() override;

		virtual std::vector<double> ProvideNetworkWithInputs() const override;

		void play();
		void AI_play(Genome & champ);

		void check_DistanceFromFood();

		GameSession();
		GameSession(int gamespeedin);
		GameSession( const GameSession& other ) = default;
};

void GameSession::check_DistanceFromFood()
{
	double DistanceFromFood = sqrt((Ekans.x.front() - pidgey.x)*(Ekans.x.front() - pidgey.x) + (Ekans.y.front() - pidgey.y)*(Ekans.y.front() - pidgey.y)) / sqrt( terminal.xmax*terminal.xmax + terminal.ymax*terminal.ymax);
	
	if( DistanceFromFood < minDistanceFromFood) 
		{
			Ekans.score++;

			minDistanceFromFood = DistanceFromFood;
		}
}

GameSession::GameSession() : gamespeed(0), gameover(false), command(0) 
{
	timeout(gamespeed);

	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);

	terminal = Board(ymax, xmax);

	xstart = terminal.xmax/2;
	ystart = terminal.ymax/2;

	Ekans  = Snake(ystart,xstart,RIGHT);
	pidgey = Food(terminal);

	minDistanceFromFood = 1.0;
}

GameSession::GameSession(int gamespeedin) : gamespeed(gamespeedin), gameover(false), command(0) 
{
	timeout(gamespeed);

	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);

	terminal = Board(ymax, xmax);

	xstart = terminal.xmax/2;
	ystart = terminal.ymax/2;

	Ekans  = Snake(ystart,xstart,RIGHT);
	pidgey = Food(terminal);

	minDistanceFromFood = 1.0;
}

void GameSession::Reset()
{
	gameover = false;
	command = 0;
	Ekans.reset(ystart, xstart, RIGHT);
	pidgey = Food(terminal);

	minDistanceFromFood = 1.0;
}

void GameSession::Update(const std::vector<double>& networkOutputs)
{    
	command = KEY_RIGHT;

		//direction is selected by choosing the outputneuron who fired the most
	auto maxelement = std::max_element(std::begin(networkOutputs), std::end(networkOutputs));
	command = std::distance(std::begin(networkOutputs), maxelement);

	if      (command == 3) command = KEY_UP;
	else if (command == 2) command = KEY_DOWN;
	else if (command == 1) command = KEY_LEFT;
	else if (command == 0) command = KEY_RIGHT;

	//clear();
	Ekans.move(command);
	//Ekans.display();
	gameover = Ekans.check_coord(terminal);	
	pidgey.check(Ekans, terminal);

	check_DistanceFromFood();
	//pidgey.display();
	//refresh();
		
	if(gameover) Reset();
}

double GameSession::GetFitness() 
{
	return Ekans.score;
}

std::vector<double> GameSession::ProvideNetworkWithInputs() const
{
	//std::vector<double> inputs;
	/*
	inputs.push_back((Ekans.y.front() - pidgey.y)/float(terminal.ymax));
	inputs.push_back((Ekans.x.front() - pidgey.x)/float(terminal.xmax));

	for(int i=Ekans.y.front() - 5; i<Ekans.y.front() + 5; i++) 
		for(int j=Ekans.x.front() - 5; j<Ekans.x.front() + 5; j++) 
		{
			float istherefood = (i == pidgey.y && j == pidgey.x)? 1.0f:0.0f;
			float isoutofbounds = (i>terminal.ymax || j>terminal.xmax)? -1.0f:0.0f;
			inputs.push_back( istherefood + isoutofbounds );
		}

	*/
	std::vector<double> inputs = {(Ekans.y.front())/float(terminal.ymax),
								 (Ekans.x.front())/float(terminal.xmax),
								 (pidgey.y)/float(terminal.xmax),
								 (pidgey.x)/float(terminal.xmax)};
								 //(Ekans.direction.front() == UP?    1.0f:0.0f),
								 //(Ekans.direction.front() == DOWN?  1.0f:0.0f),
								 //(Ekans.direction.front() == LEFT?  1.0f:0.0f), 
								 //(Ekans.direction.front() == RIGHT? 1.0f:0.0f)};
	return inputs;
}

void GameSession::play()
{
	while((command = getch()) != 'q' && !gameover)
	{
		clear();
			
		Ekans.move(command);
		Ekans.display();
		
		gameover = Ekans.check_coord(terminal);
			
		pidgey.check(Ekans, terminal);
		pidgey.display();

		refresh();
	}
	
	if(gameover) 
	{
		clear();
		//center end screen
		mvprintw(ystart-2,xstart-6, "GAME OVER");
		mvprintw(ystart  ,xstart-6, "Score: %d ", Ekans.score);
		refresh();
		sleep(2);
	}
}

void GameSession::AI_play(Genome & champ)
{
	std::vector<double> inputs;
	std::vector<double> networkOutputs;
	gameover = false;
	while((command = getch()) != 'q' && !gameover)
	{
		clear();
		inputs = ProvideNetworkWithInputs();
		networkOutputs = champ.network.FeedForward(inputs);
		
		command = 0;
		auto maxelement = std::max_element(std::begin(networkOutputs), std::end(networkOutputs));
		command = std::distance(std::begin(networkOutputs), maxelement);

		if      (command == 3) {command = KEY_UP;    mvprintw(0,0,"Command: UP");   }
		else if (command == 2) {command = KEY_DOWN;  mvprintw(0,0,"Command: DOWN"); }
		else if (command == 1) {command = KEY_LEFT;  mvprintw(0,0,"Command: LEFT"); }
		else if (command == 0) {command = KEY_RIGHT; mvprintw(0,0,"Command: RIGHT");}
		
		Ekans.move(command);
		Ekans.display();
			
		gameover = Ekans.check_coord(terminal);
				
		pidgey.check(Ekans, terminal);
		pidgey.display();
		
		refresh();
	}
	
	if(gameover) 
	{
		clear();
		//center end screen
		mvprintw(ystart-2,xstart-6, "GAME OVER");
		mvprintw(ystart  ,xstart-6, "Score: %d ", Ekans.score);
		refresh();
		//sleep(2);
	}
}
