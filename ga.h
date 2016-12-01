#include <vector>
#include <algorithm>    // std::sort
#include <fstream>
#include <memory>

#include "nn.h"

#define population 		 50
#define	elitism 		 0.2
#define randomBehaviour  0.2
#define mutationRate 	 0.1
#define mutationRange 	 0.2
#define UpdateforGen 	 4

struct ToTrain
{
	virtual std::vector<double> ProvideNetworkWithInputs() const = 0;
	virtual void Update(const std::vector<double>& networkOutputs) = 0;
	virtual void Reset() = 0;
	virtual double GetFitness() = 0;

	ToTrain() = default;
	virtual ~ToTrain() = default;
};

struct Genome
{
	Network network;
	double score;

	void ReadfromFile(const char* filename)
	{
		std::ifstream in(filename); 

		for(auto &l : network.layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;
			
			for(auto &n : l.neurons)
			{
				for(auto &w : n.weights)
					in >> w;
			}
		}
		in.close();
	}

	void SavetoFile(const char* filename)
	{
		std::ofstream output;	
		output.open(filename);

		for(auto l : network.layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;
			
			for(auto n : l.neurons)
			{
				for(auto w : n.weights)
					output << w << std::endl;
			}
		}
		output.close();
	}

	//to sort a vector of genomes
	bool operator < (const Genome& g) const
	{
		return (score < g.score);
	}

	Genome(){}
	Genome(Network n, double s) : network(n), score(s) {}
};

struct Generation 
{
	std::vector<Genome> genomes;
	std::vector<std::shared_ptr<ToTrain>> Trainees;
	
	int numberofGenerations;


	Genome breed(Genome g1, Genome g2)
	{
		Genome child = g1;
		
		for(int i=0; i<g2.network.layers.size(); i++)
			for(int j=0; j<g2.network.layers[i].neurons.size(); j++)
				for(int k=0; k<g2.network.layers[i].neurons[j].weights.size(); k++)
				{	
					double r = distr(generator);
					//std::cout << r << std::endl;
					
					if(r < 0.5)
						child.network.layers[i].neurons[j].weights[k] = g2.network.layers[i].neurons[j].weights[k];
					
					r = distr(generator);
					if(r < mutationRate)
						child.network.layers[i].neurons[j].weights[k] = distr(generator)*mutationRange*2.0 - mutationRange;
					
				}
		
		return child;
	} 

	void nextGeneration()
	{
		std::vector<Genome> nextG;

		for (int i=0; i < floor(elitism*population); i++)
			nextG.push_back(genomes[i]);
		
		for (int i=0; i < floor(randomBehaviour*population); i++)
		{
			Genome g = genomes[0];
			g.network.Randomize();

			nextG.push_back(g);
		}

		int max = 0;
		while(nextG.size() < genomes.size())
		{
			for(int i=0; i<max; i++)
				nextG.push_back(breed(genomes[i],genomes[max]));
			
			max++;
			if(max >= genomes.size()) max = 0;
		}

		//reset scores
		for(auto &genome : nextG)
		{
			genome.score = 0;
		}

		genomes = nextG;
	}
	
	void Sort()
	{
		std::sort(genomes.begin(), genomes.end());
		std::reverse(genomes.begin(),genomes.end());
	}

	double avgScore()
	{
		double avg = 0;
		for(auto &genome : genomes)
			avg += genome.score;
		return avg/population;
	}

	double avgBestScore()
	{
		double avg = 0;
		for(int i=0; i<genomes.size()/2; i++)
			avg += genomes[i].score;
		return 2*avg/population;
	}
	
	std::vector<double> EvolveFor(int goal)
	{
		std::vector<double> avg;
		
		for(int k=0; k<goal; k++)
		{
			numberofGenerations++;
			//if(!(numberofGenerations%10000)) std::cout << numberofGenerations << std::endl;
			for(int i=0; i<population; i++)
			{
				for (int j=0; j<UpdateforGen; j++)
				{
					std::vector<double> inputs = Trainees[i]->ProvideNetworkWithInputs();
					std::vector<double> output = genomes[i].network.FeedForward(inputs);

					Trainees[i]->Update(output);
				}

				genomes[i].score = Trainees[i]->GetFitness();

				Trainees[i]->Reset();
			}

			Sort();
			
			avg.push_back(avgBestScore());
			//if(genomes[0].score >= goal) break;

			if(k != (goal-1)) nextGeneration();
		}
		
		return avg;		
	}
	
	void EvolveUntilFitnessEqual(double goal)
	{
		while(1)
		{
			numberofGenerations++;
			//if(!(numberofGenerations%10000)) std::cout << numberofGenerations << std::endl;
			for(int i=0; i<population; i++)
			{
				for (int j=0; j<UpdateforGen; j++)
				{
					std::vector<double> inputs = Trainees[i]->ProvideNetworkWithInputs();
					std::vector<double> output = genomes[i].network.FeedForward(inputs);

					Trainees[i]->Update(output);
				}

				genomes[i].score = Trainees[i]->GetFitness();

				Trainees[i]->Reset();
			}

			Sort();

			if(genomes[0].score >= goal) break;

			nextGeneration();
		}
		
	}

	Generation(){}
	Generation(int nInputs, std::vector<int> nHiddens, int nOutputs) : numberofGenerations(0)
	{
		for (int i=0; i < population; i++)
		{
			Network n(nInputs,nHiddens,nOutputs);
			Genome g(n,0);
			genomes.push_back(g);
		}
	}
	//Generation(std::vector<std::shared_ptr<ToTrain>> P) : Trainees(P) {}
};