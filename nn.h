#include <iostream>
#include <math.h>
#include <vector>
#include <random>

//random generator
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> uniform_distr(-1, 1);

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
	
	std::vector<double> FeedForward(const std::vector<double> & Inputs)
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