#include <iostream>
#include <math.h>
#include <vector>
#include <random>

//random generator
std::random_device rd;
std::mt19937 generator(rd());
//std::uniform_real_distribution<double> distr(-1, 1);
std::normal_distribution<double> distr(0.0, 0.5);

double Sigmoid(double in)
{
	return  1.0/(1.0 + exp(-in));	
}

double SigmoidGradient(double in)
{
	return  Sigmoid(in)*(1.0 - Sigmoid(in));
}

struct Neuron
{
	std::vector<double> weights;
	double value;
	double delta;
	
	void Populate(int nInputs)
	{
		for(int i=0; i<nInputs; i++)
		{
			weights.push_back(distr(generator));
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
	
	void PopulateWithBias(int nNeurons, int nInputs)
	{
		for(int i=0; i < nNeurons; i++)
		{
			Neuron newNeuron(nInputs);

			neurons.push_back(newNeuron);
		}
		
		Neuron bias;
		bias.value = 1.0;
		neurons.push_back(bias);
		
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
	Layer(int ind, int nNeurons, int nInputs, bool overload)
	{
		index = ind;
		PopulateWithBias(nNeurons,nInputs);
	}
};

struct Network
{
	std::vector<Layer> layers;
	
	void perceptronGeneration(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		int previousNeurons = 0;
		int ind = 0;
		
		Layer firstLayer(ind, nInputs, previousNeurons,true);
		layers.push_back(firstLayer);
		
		ind++;
		previousNeurons = nInputs+1;
		
		//for(int i=0; i<nHiddens.size(); i++)
		for(auto nHidden : nHiddens)
		{
			Layer Hlayer(ind, nHidden, previousNeurons,true);
			previousNeurons = nHidden+1;
			layers.push_back(Hlayer);
			ind++;
		}
		
		Layer lastLayer(ind, nOutputs, previousNeurons);
		layers.push_back(lastLayer);
	}
	
	std::vector<double> FeedForward(const std::vector<double> & Inputs)
	{
		for(int i=0; i<Inputs.size(); i++) //don't update bias
			layers[0].neurons[i].value = Inputs[i];
			
		Layer previousLayer = layers[0];
		
		for(int i=1; i<layers.size()-1; i++)
		{
			//for(auto currentNeuron : layers[i].neurons)
			for(int j=0; j<layers[i].neurons.size()-1; j++) //don't update bias
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
		
		//compute last layer separately because there is no bias
		//Layer lastLayer = layers.back();
		int n = layers.size()-1;
		for(int j=0; j<layers[n].neurons.size(); j++) 
		{
			double sum = 0;
			for(int k=0; k<previousLayer.neurons.size(); k++)
			{
				sum += previousLayer.neurons[k].value*layers[n].neurons[j].weights[k];
			}
			layers[n].neurons[j].value = Sigmoid(sum);
		}
		
		std::vector<double> output;
		for(auto neuron : layers[n].neurons)
			output.push_back(neuron.value);
			
		return output;
	}
	
	void BackProgagation(double LearningRate, const std::vector<double> & Truth)
	{
		//do output layer
		int n = layers.size()-1;
		for(int j=0; j<layers[n].neurons.size(); j++) 
		{
			layers[n].neurons[j].delta = layers[n].neurons[j].value*(1.0 - layers[n].neurons[j].value)*(Truth[j] - layers[n].neurons[j].value);
		}
		
		//hidden layer from output layer (no bias)
		for(int k=0; k<layers[n-1].neurons.size(); k++) //previous layer
		{
			double sum = 0;
			for(int j=0; j<layers[n].neurons.size(); j++) 
			{
				sum += layers[n].neurons[j].weights[k]*layers[n].neurons[j].delta;
			}
				
			layers[n-1].neurons[k].delta = layers[n-1].neurons[k].value*(1.0 - layers[n-1].neurons[k].value)*sum;
		}

		//hidden layers
		for(int i=n-1; i>1; i--)
		{
			for(int k=0; k<layers[i-1].neurons.size(); k++) //previous layer
			{
				double sum = 0;
				for(int j=0; j<layers[i].neurons.size()-1; j++) 
				{
					sum += layers[i].neurons[j].weights[k]*layers[i].neurons[j].delta;
				}
				
				layers[i-1].neurons[k].delta = layers[i-1].neurons[k].value*(1.0 - layers[i-1].neurons[k].value)*sum;
			}
		}

		//update output layer's weights
		for(int j=0; j<layers[n].neurons.size(); j++) 
		{
			for(int k=0; k<layers[n-1].neurons.size(); k++) //previous layer
			{
				layers[n].neurons[j].weights[k] += LearningRate*layers[n].neurons[j].delta*layers[n-1].neurons[k].value;
			}
		}

		//update all other weights
		for(int i=n-1; i>0; i--)
		{
			for(int j=0; j<layers[i].neurons.size()-1; j++) //don't update weights
			{
				for(int k=0; k<layers[i-1].neurons.size(); k++) //previous layer
				{
					layers[i].neurons[j].weights[k] += LearningRate*layers[i].neurons[j].delta*layers[i-1].neurons[k].value;
				}
			}
		}
			
	}
	
	void Randomize()
	{
		for(auto &l : layers)
		{
			//skip input layer that has no weights
			if(l.index == 0) continue;

			for(auto &n : l.neurons)
				for(auto &w : n.weights)
					w = distr(generator);
		}
	}

	void print()
	{
		std::cout << "hello world, i am a neural network and my layers are = " << std::endl;
		for(auto l : layers)
		{
			//skip input layer that has no weights
			//if(l.index == 0) continue;
			
			std::cout << "\t hello world, i am layer number " << l.index << " and my neurons are = " << std::endl;
			int j=0;
			for(auto n : l.neurons)
			{
				std::cout << "\t \t hello world, i am neuron number " << j << " and my weights are = " << std::endl;
				j++;
				for(auto w : n.weights)
					std::cout << "\t \t " << w << std::endl;
				std::cout << "\t \t and my value is  = " << n.value << std::endl;
			}
		}
	}
	
	
	Network(){}
	Network(int nInputs, std::vector<int> nHiddens, int nOutputs)
	{
		perceptronGeneration(nInputs, nHiddens, nOutputs);
	}
};