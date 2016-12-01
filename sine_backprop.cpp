#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "nn.h"

#define PI 3.14159265358979323846264

enum{ N = 100 };

int main()
{
	//random generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0, N - 1);

	std::vector<double> dataset;
	std::vector<double> truth;
	//creating training set
	
	for(int i=0; i<N; i++)
	{
		dataset.push_back(double(2*PI*i)/N);
		truth.push_back(0.5*(sin(dataset[i]) + 1.0));
	}
	
	Network nn(1,{20},1);
	//nn.print();
		
	double alpha = 0.25;
	int Niter = 10000;
	
	std::cout << "learning rate = " << alpha << std::endl;
	std::cout << "# iterations = " << Niter << std::endl;
	std::cout << std::endl;
	
	std::cout << "Learning the function 1/2 (sin(x) + 1) " << std::endl;
	std::cout << "Training..." << std::endl;
	
	double r;
	for(int i=0; i<Niter; i++)
	{
		for(int j=0; j<N; j++)
		{
			r = distribution(generator);
			
			nn.FeedForward({dataset[r]});
			nn.BackProgagation(alpha, {truth[r]});		
		}
	}
	std::cout << std::endl;
	nn.print();
	std::cout << std::endl;
	
	std::cout << "Testing the trained network..." << std::endl;
	std::cout << std::endl;
	
	std::vector<double> results;
	for(int i=0; i<N; i++)
	{
		auto c = nn.FeedForward({dataset[i]});
		results.push_back(c[0]);
	}
	
	std::cout << "angle(rad)" << "    " << "predicted" << "    " << "truth" << std::endl;
	std::cout << std::endl;
	for(int i=0; i<N; i++)
		std::cout << dataset[i] << "    " << results[i] << "    " << truth[i] << std::endl;
	
	
	double E = 0;
	for(int i=0; i<N; i++)
	{
		E += (results[i] - truth[i])*(results[i] - truth[i]);
	}
	std::cout << "total error = " << sqrt(E) << std::endl;
	
	return 0;
}