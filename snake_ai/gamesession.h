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

		//double minDistanceFromFood;
		//snake starting point
		int xstart, ystart;
		int gamespeed; // bigger = slower (gamespeed->delay in millisecond)

		bool gameover;
		int command;

		virtual void Reset() override;
		virtual bool Update(const std::vector<double>& networkOutputs) override;
		virtual double GetFitness() override;

		virtual std::vector<double> ProvideNetworkWithInputs() const override;

		void play();
		void AI_play(Genome & champ);

		void LookinDirection(int _direction_x, int _direction_y, double& in_food, double& in_body, double& in_wall) const;

		GameSession();
		GameSession(int gamespeedin);
		GameSession( const GameSession& other ) = default;
};

// void GameSession::check_DistanceFromFood()
// {
// 	double DistanceFromFood = sqrt((Ekans.x.front() - pidgey.x)*(Ekans.x.front() - pidgey.x) + (Ekans.y.front() - pidgey.y)*(Ekans.y.front() - pidgey.y)) / sqrt( terminal.xmax*terminal.xmax + terminal.ymax*terminal.ymax);
	
// 	if( DistanceFromFood < minDistanceFromFood) 
// 		{
// 			Ekans.score++;

// 			minDistanceFromFood = DistanceFromFood;
// 		}
// }

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

	//minDistanceFromFood = 1.0;
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

	//minDistanceFromFood = 1.0;
}

void GameSession::Reset()
{
	gameover = false;
	command = 0;
	Ekans.reset(ystart, xstart, RIGHT);
	pidgey = Food(terminal);

	//minDistanceFromFood = 1.0;
}

bool GameSession::Update(const std::vector<double>& networkOutputs)
{    
	command = KEY_RIGHT;

	//std::this_thread::sleep_for (std::chrono::seconds(1));

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

	//check_DistanceFromFood();
	//pidgey.display();
	//refresh();
		
	//if(gameover) Reset();

	//avoid playing forever
	if(Ekans.lifetime > (Ekans.size-2)*150) gameover = true;

	return gameover;
}

double GameSession::GetFitness() 
{
	//fitness is based on length and lifetime
	double fitness = 0.0;
	int length = Ekans.size;

    if (length < 10) 
    {
      	fitness = Ekans.lifetime * Ekans.lifetime * pow(2, length);
    } 
    else 
    {
      	//grows slower after 10 to stop fitness from getting too big
     	 //ensure length greater than 9
      	fitness = Ekans.lifetime * Ekans.lifetime;
      	fitness *= pow(2, 10);
      	fitness *=(length - 9);
	}
	return fitness;
}

void GameSession::LookinDirection(int _direction_x, int _direction_y, double& in_food, double& in_body, double& in_wall) const
{
	//position where we are looking for food
	int _position_x = Ekans.x.front();
	int _position_y = Ekans.y.front();

	bool foodIsFound = false; //true if the food has been located in the direction looked
	bool tailIsFound = false;

	double distance = 0;

	_position_x += _direction_x;
	_position_y += _direction_y;
	
	distance += 1;

	in_food = 0.0;
	in_body = 0.0;
	in_wall = 0.0;

	while(!( _position_x < 0 || _position_x >= terminal.xmax || _position_y < 0 || _position_y >= terminal.ymax ))
	{

		if (!foodIsFound && _position_x == pidgey.x && _position_y == pidgey.y) 
		{
			in_food = 1;
			foodIsFound = true; // dont check if food is already found
		}

		//check for tail at the position
		if (!tailIsFound && Ekans.check_body(_position_x, _position_y)) 
		{
			in_body = 1/distance;
			tailIsFound = true; // dont check if tail is already found
		}


		_position_x += _direction_x;
		_position_y += _direction_y;
	
		distance += 1;
	}

	in_wall = 1/distance;
}

std::vector<double> GameSession::ProvideNetworkWithInputs() const
{
	std::vector<double> inputs;

	double food_dist = 0.0;
	double body_dist = 0.0;
	double wall_dist = 0.0;

	LookinDirection( 1, 1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection( 1, 0, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection( 1,-1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection(-1,-1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection(-1, 0, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection(-1, 1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection( 0, 1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);
	LookinDirection( 0,-1, food_dist, body_dist, wall_dist);
	inputs.push_back(food_dist); 
	inputs.push_back(body_dist); 
	inputs.push_back(wall_dist);

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
		sleep(2);
	}
}
