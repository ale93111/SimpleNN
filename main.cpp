#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "ga.h"


int main()
{
	//XOR TEST
	Generation X(2,{3},1);

	std::vector<std::vector<double>> dataset = {{1,0},{1,1},{0,1},{0,0}};

	std::vector<double> truth = {1,0,1,0};
	
	int count = 0;
	std::vector<double> avgscore;

	while(1)
	{
		count++;
		for(auto &genome : X.genomes)
		{
			std::vector<double> output;
			for(auto input : dataset)
			{
				auto c = genome.network.FeedForward(input);
				output.push_back(c[0]>0.5);
			}
			
			for(int i=0; i<4; i++)
				if(output[i] == truth[i]) genome.score++;
		}

		X.Sort();

		//if(!(count%100))
		//	avgscore.push_back(X.avgScore());
		

		if(X.genomes[0].score == 4) break;

		X.nextGeneration();
	}

	std::cout << "XOR solved!!" << std::endl;
	std::cout << "number of generations = " << count << std::endl;

	Genome champ = X.genomes[0];
	//revaluate champ
	std::vector<double> output;
	for(auto input : dataset)
	{
		auto c = champ.network.FeedForward(input);
		output.push_back(c[0]);
	}

	for(int i=0; i<output.size(); i++)
	{
		std::cout<< "the output of {" << dataset[i][0] << "," << dataset[i][1] << "} is " << output[i] << std::endl;		
	}

	champ.SavetoFile("champ.nn");
	champ.network.print();

	Network t(2,{3},1);
	Genome test(t,0);

	test.ReadfromFile("champ.nn");
	test.network.print();


	/*
	std::cout << std::endl;
	for(auto &genome : X.genomes)
		std::cout << genome.score;

	std::cout << std::endl;
	
	std::ofstream output;	
	output.open("scores.txt");
	
	for(int i=0; i<avgscore.size(); i++)
	{
		output << i << "\t" << avgscore[i] << std::endl; 	
	}
	
	output.close();
	*/

	return 0;
}