#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>    // std::sort
#include <fstream>
#include <sstream>

#include "nn.h"

void read_csv( std::vector<std::vector<double>> & data, std::vector<double> & labels, std::string filename)
{
	//training_data = {};
	//labels        = {};
	//expects the label at the start of each line, followed by the input data
	std::ifstream infile(filename); 
    std::string line;
	
    while (getline(infile, line))
	{
		std::stringstream strstr(line);
        std::string value;
		
		getline(strstr,value, ',');
		labels.push_back(std::stoi(value));

		std::vector<double> single_digit;
        while (getline(strstr,value, ',')) 
		{
			//std::cout << word;   
			single_digit.push_back(std::stoi(value));
		}
		data.push_back(single_digit);
	}
}

void print_digit(int y, const std::vector<std::vector<double>> & data, const std::vector<double> & labels)
{
	for( int i=0; i<28; i++)
	{
		for(int j=0; j<28; j++)
		{
			if(data[y][i*28+j] > 0)
				std::cout << "@";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	//std::cout << std::endl;
	std::cout << "label = " << labels[y] << std::endl;
}

int main()
{
	std::string train_filename = "mnist/mnist_train.csv";
	std::string test_filename  = "mnist/mnist_test.csv";
	
	std::vector<std::vector<double>> training_data;
	std::vector<double> train_labels;
	
	std::vector<std::vector<double>> test_data;
	std::vector<double> test_labels;
	
	std::cout << "Loading train set..." << std::endl;
	read_csv(training_data, train_labels, train_filename);
	
	std::cout << "# labels = " << train_labels.size() << std::endl;
	std::cout << "# digits = " << training_data.size() << std::endl;
	std::cout << "# pixels = " << training_data[0].size() << std::endl;
	std::cout << std::endl;
	
	std::cout << "Loading test set..." << std::endl;
	read_csv(test_data, test_labels, test_filename);
	
	std::cout << "# labels = " << test_labels.size() << std::endl;
	std::cout << "# digits = " << test_data.size() << std::endl;
	std::cout << "# pixels = " << test_data[0].size() << std::endl;
	std::cout << std::endl;
	
	std::cout << "Dataset normalization..." << std::endl;
	std::cout << std::endl;
	
	for(auto &data : training_data)
		for(auto &val : data)
		{
			val /= 255.0;
		}
	for(auto &data : test_data)
		for(auto &val : data)
		{
			val /= 255.0;
		}
	
	//make label vector
	std::vector<std::vector<double>> vec_labels;
	for(auto label : train_labels)
	{
		std::vector<double> vec_label(10,0);
		vec_label[label]++;
		vec_labels.push_back(vec_label);
	}	
	
	int input_size = training_data[0].size();
	int train_size = test_data.size();
	
	Network nn(input_size,{250},10);
	
	double alpha = 0.05;
	int Nepoch = 5;
	
	std::cout << "learning rate = " << alpha << std::endl;
	std::cout << "# epochs = " << Nepoch << std::endl;
	std::cout << std::endl;
	
	std::cout << "Learning the MNIST dataset " << std::endl;
	std::cout << "Training..." << std::endl;

	std::cout << "NOTA:  using the first 10k samples of the training set" << std::endl;
	//IMPORTANT: using test data size to use only the first 10000 digits of the train dataset, to speedup computation
	//random generator
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> distribution(0, train_size - 1);
	
	double r;
	for(int i=0; i<Nepoch; i++)
	{
		for(int j=0; j<train_size; j++)
		{
			r = distribution(generator);
			
			nn.FeedForward(training_data[r]);
			nn.BackProgagation(alpha,vec_labels[r]);		
		}
		if(!(i%1)) std::cout << "epoch " << i+1 << "..." << std::endl;
	}
	std::cout << std::endl;
	
	std::cout << "Testing the trained network..." << std::endl;
	
	std::vector<double> test_results;
	for(int i=0; i<test_data.size(); i++)
	{
		auto res = nn.FeedForward(test_data[i]);
		//select the neuron who fired the most
		auto maxelement = std::max_element(std::begin(res), std::end(res));
		auto c = std::distance(std::begin(res), maxelement);
		test_results.push_back(c);
	}
	
	int n_correct = 0;
	for(int i=0; i<test_data.size(); i++)
	{
		if( test_labels[i] == test_results[i]) 
			n_correct++;
	}
	
	std::cout << "Final accuracy on the test set = " << double(n_correct)/test_data.size() << std::endl;
	
	//IMPORTANT: using test data size to use only the first 10000 digits of the train dataset, to speedup computation
	std::vector<double> train_results;
	for(int i=0; i<train_size; i++)
	{
		auto res = nn.FeedForward(training_data[i]);
		//select the neuron who fired the most
		auto maxelement = std::max_element(std::begin(res), std::end(res));
		auto c = std::distance(std::begin(res), maxelement);
		train_results.push_back(c);
	}
	
	n_correct = 0;
	for(int i=0; i<train_size; i++)
	{
		if( train_labels[i] == train_results[i]) 
			n_correct++;
	}
	
	std::cout << "Final accuracy on the train set = " << double(n_correct)/train_size << std::endl;
	
	for(int i=0; i<1; i++)
	{
		r = distribution(generator);
		print_digit(r,test_data,test_labels);
		std::cout << "predicted label = " << test_results[r] << std::endl;
	}
	
	
	
	return 0;
}