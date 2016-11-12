#include <iostream>
//#include <math.h>
#include <vector>
#include <random>
#include <functional>

//random generator
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> uniform_distr(-1, 1);

struct Neuron
{
	std::vector<double> weights;
	
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
	
	Network(){}
	Network(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		perceptronGeneration(nInputs, nHiddens, nOutputs);
	}
};

int main()
{
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
	
	std::cout << "hello world, i am a layer and my neurons are = " << std::endl;
	

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
	
	
	for(auto l : network.layers)
	{
		std::cout << "hello world, i am layer number " << l.index << " and my neurons are = " << std::endl;
		
		for(auto n : l.neurons)
		{
			std::cout << "hello world, i am neuron number " << j << " and my weights are = " << std::endl;
			j++;
			for(auto w : n.weights)
				std::cout << w << std::endl;
		}
	}
	j=0;
	
	return 0;
}