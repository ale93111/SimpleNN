#include <vector>
#include <algorithm>    // std::sort
#include <fstream>

#include "nn.h"

#define population 		 100
#define	elitism 		 0.2
#define randomBehaviour  0.2
#define mutationRate 	 0.1
#define mutationRange 	 0.2

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
	
	
	Genome breed(Genome g1, Genome g2)
	{
		Genome child = g1;
		
		for(int i=0; i<g2.network.layers.size(); i++)
			for(int j=0; j<g2.network.layers[i].neurons.size(); j++)
				for(int k=0; k<g2.network.layers[i].neurons[j].weights.size(); k++)
				{	
					double r = uniform_distr(generator);
					//std::cout << r << std::endl;
					
					if(r < 0.5)
						child.network.layers[i].neurons[j].weights[k] = g2.network.layers[i].neurons[j].weights[k];
					
					r = uniform_distr(generator);
					if(r < mutationRate)
						child.network.layers[i].neurons[j].weights[k] = uniform_distr(generator)*mutationRange*2.0 - mutationRange;
					
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

	Generation(){}
	Generation(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		for (int i=0; i < population; i++)
		{
			Network n(nInputs,nHiddens,nOutputs);
			Genome g(n,0);
			genomes.push_back(g);
		}
	}
};