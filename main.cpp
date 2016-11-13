#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <algorithm>    // std::sort
#include <fstream>

//random generator
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> uniform_distr(-1, 1);


#define population 		 100
#define	elitism 		 0.2
#define randomBehaviour  0.2
#define mutationRate 	 0.1
#define mutationRange 	 0.2

double Sigmoid(double in)
{
	return  1.0/(1.0 + exp(-in));	
}

struct Neuron
{
	std::vector<double> weights;
	double value;
	
	void Populate(int nInputs)
	{
		for(int i=0; i<nInputs; i++)
		{
			weights.push_back(uniform_distr(generator));
		}
	}
	
	Neuron(){}
	Neuron(int nInputs)
	{
		Populate(nInputs);
	}
	
};

struct Layer
{
	int index;
	std::vector<Neuron> neurons;
	
	void Populate(int nNeurons, int nInputs)
	{
		for(int i=0; i < nNeurons; i++)
		{
			Neuron newNeuron;
			newNeuron.Populate(nInputs);
			
			neurons.push_back(newNeuron);
		}
	}
				
	Layer(){}
	Layer(int nNeurons, int nInputs)
	{
		Populate(nNeurons,nInputs);
	}
	Layer(int ind, int nNeurons, int nInputs)
	{
		index = ind;
		Populate(nNeurons,nInputs);
	}
};

struct Network
{
	std::vector<Layer> layers;
	
	void perceptronGeneration(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		int previousNeurons = 0;
		int ind = 0;
		
		Layer firstLayer(ind, nInputs, previousNeurons);
		layers.push_back(firstLayer);
		
		ind++;
		previousNeurons = nInputs;
		
		//for(int i=0; i<nHiddens.size(); i++)
		for(auto nHidden : nHiddens)
		{
			Layer Hlayer(ind, nHidden, previousNeurons);
			previousNeurons = nHidden;
			layers.push_back(Hlayer);
			ind++;
		}
		
		Layer lastLayer(ind, nOutputs, previousNeurons);
		layers.push_back(lastLayer);
	}
	
	std::vector<double> FeedForward(std::vector<double> Inputs)
	{
		for(int i=0; i<Inputs.size(); i++)
			layers[0].neurons[i].value = Inputs[i];
			
		Layer previousLayer = layers[0];
		
		for(int i=1; i<layers.size(); i++)
		{
			//for(auto currentNeuron : layers[i].neurons)
			for(int j=0; j<layers[i].neurons.size(); j++)
			{
				double sum = 0;
				//for(auto previousNeuron : previousLayer.neurons)
				for(int k=0; k<previousLayer.neurons.size(); k++)
				{
					sum += previousLayer.neurons[k].value*layers[i].neurons[j].weights[k];
				}
				layers[i].neurons[j].value = Sigmoid(sum);
			}
			
			previousLayer = layers[i];
		}
		
		std::vector<double> output;
		
		Layer lastLayer = layers.back();
		for(auto neuron : lastLayer.neurons)
			output.push_back(neuron.value);
			
		return output;
	}
	
	void Randomize()
	{
		for(auto &l : layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;

			for(auto &n : l.neurons)
				for(auto &w : n.weights)
					w = uniform_distr(generator);
		}
	}

	void print()
	{
		std::cout << "hello world, i am a neural network and my layers are = " << std::endl;
		for(auto l : layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;
			
			std::cout << "hello world, i am layer number " << l.index << " and my neurons are = " << std::endl;
			int j=0;
			for(auto n : l.neurons)
			{
				std::cout << "hello world, i am neuron number " << j << " and my weights are = " << std::endl;
				j++;
				for(auto w : n.weights)
					std::cout << w << std::endl;
			}
		}
	}
	
	
	Network(){}
	Network(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		perceptronGeneration(nInputs, nHiddens, nOutputs);
	}
};

struct Genome
{
	Network network;
	double score;
	void ReadfromFile(const char* filename)
	{
		//std::vector<double> weights;

		std::ifstream in(filename); 

		double number; 
		
	    //while (in >> number) { 
		//	weights.push_back(number);
		//}

		

		//int i = 0;
		for(auto &l : network.layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;
			
			for(auto &n : l.neurons)
			{
				for(auto &w : n.weights)
					in >> w;// = weights[i++];
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
	
	//void addGenome(){} //TODO add genome and sort score
	
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