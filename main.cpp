#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <functional>

//random generator
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> uniform_distr(-1, 1);


int 	population 		= 50;
double	elitism 		= 0.2;
double 	randomBehaviour = 0.2;
double 	mutationRate 	= 0.1;
double 	mutationRange 	= 0.5;

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
	
	Genome(){}
	Genome(Network n, double s) : network(n), score(s) {}
};

struct Generation
{
	std::vector<Genome> networks;
	
	void addGenome(){} //TODO add genome and sort score
	
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
	
	Generation(){}
};

int main()
{
	Network n1(2,{2},2);
	Network n2(2,{2},2);
	
	//n1.print();
	//n2.print();
	
	Genome g1(n1,0), g2(n2,0);
		
	g1.network.print();
	g2.network.print();
	
	Generation X;
	
	auto xman = X.breed(g1,g2);
	
	xman.network.print();
	
	/*
	Neuron neuron;
	neuron.Populate(2);
	
	for(auto weight : neuron.weights)
		std::cout << "hello world, i am a neuron and my weights are = " << weight << std::endl;
	
	Neuron anotherNeuron;
	anotherNeuron.Populate(2);
	
	for(auto weight : anotherNeuron.weights)
		std::cout << "hello world, i am a another neuron and my weight is = " << weight << std::endl;
	
	std::cout << std::endl;
	
	Layer layer(0,5,2);
	//layer.Populate(5,2);
	
	std::cout << "hello world, i am a layer " << layer.index << " and my neurons are = " << std::endl;
	

	int j = 0;
	for(auto n : layer.neurons)
	{
		std::cout << "hello world, i am neuron number " << j << " and my weights are = " << std::endl;
		j++;
		for(auto w : n.weights)
			std::cout << w << std::endl;
	}
	j=0;
	
	std::cout << std::endl;
	
	std::vector<int> nHiddens = {3};
	Network network(3,nHiddens,3);
	
	std::cout << "hello world, i am a neural network and my layers are = " << std::endl;
	
	
	
	
	std::cout << std::endl;
	
	std::vector<double> input = {1,1,1};
	std::cout << "here is my list of inputs = " << std::endl;
	for(auto in : input) std::cout << in << std::endl;
	
	auto output = network.FeedForward(input);
	
	std::cout << "and here is my result = " << std::endl;
	for(auto out : output) std::cout << out << std::endl;
	*/
	return 0;
}