#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "nn.h"


int main()
{
	//random generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0,3);
	
	std::vector<std::vector<double>> dataset = {{1,0},{1,1},{0,1},{0,0}};
	std::vector<double> truth = {1,0,1,0};
	
	Network nnn(2,{4},1);
	
	std::cout << "Testing the initial random network..." << std::endl;
	std::cout << "output of 1,0 = " << nnn.FeedForward({1,0})[0] << std::endl;
	std::cout << "output of 1,1 = " << nnn.FeedForward({1,1})[0] << std::endl;
	std::cout << "output of 0,1 = " << nnn.FeedForward({0,1})[0] << std::endl;
	std::cout << "output of 0,0 = " << nnn.FeedForward({0,0})[0] << std::endl;
	std::cout << std::endl;
	//nnn.print();
	
	
	double alpha = 0.8;
	int Niter = 3000;
	double r;
	
	std::cout << "learning rate = " << alpha << std::endl;
	std::cout << "# iterations = " << Niter << std::endl;
	std::cout << std::endl;
	
	std::cout << "Training..." << std::endl;
	for(int i=0; i<Niter; i++)
	{
		for(int j=0; j<4; j++)
		{
			r = distribution(generator);
			
			nnn.FeedForward(dataset[r]);
			nnn.BackProgagation(alpha, {truth[r]});		
		}
	}
	std::cout << std::endl;
	nnn.print();
	std::cout << std::endl;
	std::cout << "Testing the final trained network..." << std::endl;
	std::cout << "output of 1,0 " << nnn.FeedForward({1,0})[0] << std::endl;
	std::cout << "output of 1,1 " << nnn.FeedForward({1,1})[0] << std::endl;
	std::cout << "output of 0,1 " << nnn.FeedForward({0,1})[0] << std::endl;
	std::cout << "output of 0,0 " << nnn.FeedForward({0,0})[0] << std::endl;
	std::cout << std::endl;
	
	return 0;
}