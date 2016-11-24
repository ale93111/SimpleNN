#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "ga.h"

#define PI 3.14159265358979323846264

int main()
{
	//random generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0,3);
	
	std::vector<std::vector<double>> dataset = {{1,0},{1,1},{0,1},{0,0}};
	std::vector<double> truth = {1,0,1,0};
	
	Network nnn(2,{4},1);
	
	std::cout << "output di 1,0 " << nnn.FeedForward({1,0})[0] << std::endl;
	std::cout << "output di 1,1 " << nnn.FeedForward({1,1})[0] << std::endl;
	std::cout << "output di 0,1 " << nnn.FeedForward({0,1})[0] << std::endl;
	std::cout << "output di 0,0 " << nnn.FeedForward({0,0})[0] << std::endl;
	//nnn.print();
	
	
	double alpha = 0.8;
	int Niter = 3000;
	double r;
	for(int i=0; i<Niter; i++)
	{
		for(int j=0; j<4; j++)
		{
			r = distribution(generator);
			
			nnn.FeedForward(dataset[r]);
			nnn.BackProgagation(alpha, {truth[r]});		
		}
	}
	nnn.print();
	std::cout << "output di 1,0 " << nnn.FeedForward({1,0})[0] << std::endl;
	std::cout << "output di 1,1 " << nnn.FeedForward({1,1})[0] << std::endl;
	std::cout << "output di 0,1 " << nnn.FeedForward({0,1})[0] << std::endl;
	std::cout << "output di 0,0 " << nnn.FeedForward({0,0})[0] << std::endl;
	
	
	/*
	//random generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0,99);

	std::vector<double> dataset;
	std::vector<double> truth;
	for(int i=0; i<100; i++)
	{
		dataset.push_back(double(2*PI*i)/100.0);
		truth.push_back(0.5*(sin(dataset[i]) + 1.0));
	}
	
	Network nnn(1,{20},1);
	nnn.print();
		
	double alpha = 0.25;
	int Niter = 100000;
	double r;
	for(int i=0; i<Niter; i++)
	{
		for(int j=0; j<100; j++)
		{
			r = distribution(generator);
			
			nnn.FeedForward({dataset[r]});
			nnn.BackProgagation(alpha, {truth[r]});		
		}
	}
	nnn.print();
	
	std::vector<double> results;
	for(int i=0; i<100; i++)
	{
		auto c = nnn.FeedForward({dataset[i]});
		results.push_back(c[0]);
	}
		
	for(int i=0; i<100; i++)
		std::cout << dataset[i] << "    " << results[i] << "    " << truth[i] << std::endl;
	
	double E = 0;
	for(int i=0; i<100; i++)
	{
		E += (results[i] - truth[i])*(results[i] - truth[i]);
	}
	std::cout << "total error = " << E << std::endl;
	*/
	return 0;
}