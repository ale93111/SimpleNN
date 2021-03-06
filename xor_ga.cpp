#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "ga.h"

bool XOR(bool a, bool b)
{
    return (a + b) % 2;
}

struct XORSolver : public ToTrain
{
	std::vector<std::pair<int,int>> dataset = {{1,0},{1,1},{0,1},{0,0}};
	std::vector<std::pair<int,int>>::iterator currTraining = dataset.begin();

	std::vector<int> truth = {1,0,1,0};
	std::vector<double> results;

	virtual void Update(const std::vector<double>& networkOutputs) override;
	virtual void Reset() override;
	virtual double GetFitness() override;
	virtual std::vector<double> ProvideNetworkWithInputs() const override;

	XORSolver(){}
};

void XORSolver::Update(const std::vector<double>& networkOutputs)
{    
	int xorResult = XOR(currTraining->first, currTraining->second);
	int networksXorResult = int(networkOutputs.front() >= 0.5f);

	results.push_back(networksXorResult);
	++currTraining;
	if (currTraining == dataset.end()) {
		currTraining = dataset.begin();
	}
}

void XORSolver::Reset()
{
	currTraining = dataset.begin();
	results.clear();
}

double XORSolver::GetFitness() 
{
	std::size_t correctEvaluations = 0;
	auto firstResult = results.front();
	bool areAllResultsSame = true;
	for (std::size_t i = 0; i < dataset.size(); ++i) {
		auto correctResult = XOR(dataset[i].first, dataset[i].second);
		if (results[i] == correctResult) {
			correctEvaluations++;
		}
		if (results[i] != firstResult) {
			areAllResultsSame = false;
		}
	}
	if (areAllResultsSame) {
		return 0.0;
	}
	return double(correctEvaluations * correctEvaluations);
}

std::vector<double> XORSolver::ProvideNetworkWithInputs() const
{
	std::vector<double> inputs = {(double)currTraining->first, (double)currTraining->second};
	return inputs;
}

int main()
{
	//XOR TEST
	std::vector<std::vector<double>> dataset = {{1,0},{1,1},{0,1},{0,0}};
	
	std::cout << "Initializing the first generation..." << std::endl << std::endl;
	Generation X(2,{3},1);

	for(int i=0; i<population; i++)
		X.Trainees.push_back(std::make_shared<XORSolver>());
		
	
	std::cout << "Testing a random neural network..." << std::endl;
	Genome loser = X.genomes[0];
	std::vector<double> output;
	for(auto input : dataset)
	{
		auto c = loser.network.FeedForward(input);
		output.push_back(c[0]);
	}

	for(int i=0; i<output.size(); i++)
		std::cout<< "the output of {" << dataset[i][0] << "," << dataset[i][1] << "} is " << output[i] << " ( = " << int(output[i]>0.5) << " )" << std::endl;		
	std::cout << std::endl;
	
	
	std::cout << "Learning XOR" << std::endl;
	std::cout << "Genetic algorithm starting..." << std::endl << std::endl;
	
	X.EvolveUntilFitnessEqual(16.0);
	
	std::cout << "XOR solved!!" << std::endl;
	std::cout << "# of generations = " << X.numberofGenerations << std::endl << std::endl;
	
	std::cout << "Testing the winner..." << std::endl << std::endl;
	Genome champ = X.genomes[0];
	//revaluate champ
	output = {};
	for(auto input : dataset)
	{
		auto c = champ.network.FeedForward(input);
		output.push_back(c[0]);
	}

	for(int i=0; i<output.size(); i++)
		std::cout<< "the output of {" << dataset[i][0] << "," << dataset[i][1] << "} is " << output[i] << " ( = " << int(output[i]>0.5) << " )" << std::endl;		
	std::cout << std::endl;
	//champ.SavetoFile("xor_champ.nn");
	champ.network.print();

	return 0;
}