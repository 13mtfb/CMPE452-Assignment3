// CMPE452_Assignment3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//each sample is a two-dimensional
//vector of doubles
struct sample {
	double a;
	double b;
};

//assumes two double values separated by a single comma:
sample parseInput(std::string line) {
	//sample return
	sample input;

	//substring to parse csv
	std::string subString;
	//values for string find
	int firstPos = 0;
	int subPos;
	double value;

	//parse float values
	for (int i = 0; i < 2; i++) {
		subPos = line.find(",", firstPos);
		subString = line.substr(firstPos, subPos);
		firstPos = subPos + 1;
		
		value = atof(subString.c_str());
		if (i == 0) { input.a = value;  }
		if (i == 1) { input.b = value;  }

		}

	return input;
}

using namespace std;

int main()
{
	// Import data as a vector containing a struct which contains two doubles
	vector <sample> input;
	// Create a second vector for calculating the output samples
	vector <sample> calcOutput;

	// Count number of training samples
	int countData = 0;

	//declare an input filestream to read data
	ifstream data;
	//Create an output filestream to write data
	ofstream outFile("output.csv");
	//create another output file to format for conversion to wav
	ofstream toWav("toWav.csv");
	//open the file stream
	data.open("sound.csv");

	if (!data) {
		cerr << "Unable to open file" << endl;
		exit(1);
	}

	//iterate through the file, line-by-line
	for (string line; getline(data, line); ) {
		//create sample struct to push to vector
		sample addData;
		//increment count
		countData++;
		//parse string line
		addData = parseInput(line);

		//Print data
		//cout << addData.a << ", " << addData.b << endl;

		//push the struct to the vector
		input.push_back(addData);
	}

	cout << "Number of training samples: " << countData << endl;



	// Subtract the mean vector from each sample

	//first assign the output vector to the original vector
	calcOutput = input;

	//Determine the total of the input samples 
	double average[2] = { 0,0 };
	for (int i = 0; i < countData; i++) {
		average[0] = average[0] + input[i].a;
		average[1] = average[1] + input[i].b;
	}
	//Divide by the number of training samples
	average[0] = average[0] / countData;
	average[1] = average[1] / countData;
	//cout << average[0] << ", " << average[1] << endl;
	//Subtract the average from each training sample
	for (int i = 0; i < countData; i++) {
		input[i].a = input[i].a - average[0];
		input[i].b = input[i].b - average[1];
	}

	//define a weight vector with random initial values
	double weight[2] = { 0.2, 0.2 };
	//define a delta weight vector
	double deltaWeight[2] = { 0, 0 };
	//define a learning rate
	double learningRate = 0.1;
	//define the output Y
	double output;

	int numIterations = 10;

	cout << "Initial weights: ";
	cout << weight[0] << ", " << weight[1] << endl;

	// For each iteration:
	//	For each training sample:
	//		Calculate the output Y = w1x1 + w2x2
	//		Calculate deltaWeight = n * Y*(x1, x2) - Y^2(w1, w2)
	//		Calculate the Weight = Weight + deltaWeight

	//loop through number of iterations
	for (int i = 0; i < numIterations; i++) {
		//loop through number of training samples
		for (int j = 0; j < countData; j++) {
			//Calculate the output Y = w1x1 + w2x2
			output = weight[0] * input[j].a + weight[1] * input[j].b;
			//Calculate deltaWeight = n * Y*(x1, x2) - Y^2(w1, w2)
			deltaWeight[0] = (learningRate * output * input[j].a) - (pow(output, 2.0) * weight[0]);
			deltaWeight[1] = (learningRate * output * input[j].b) - (pow(output, 2.0) * weight[1]);
			//Calculate the new Weight = Weight + deltaWeight
			weight[0] = weight[0] + deltaWeight[0];
			weight[1] = weight[1] + deltaWeight[1];

			//cout << weight[0] << ", " << weight[1] << endl;
		}

		cout << "Weights after iteration " << i << ": ";
		cout << weight[0] << ", " << weight[1] << endl;
	}

	/////////////////////////////////////////////////////////////////////////////
	////////////////////////Training Complete////////////////////////////////////

	//Calculate the PCA from the weights determined above and the original input samples

	for (int i = 0; i < countData; i++) {
		//Write result to output csv
		outFile << calcOutput[i].a * weight[0] + calcOutput[i].b * weight[1] << ",";
		//Write different formatting to output csv
		toWav << "0, " << calcOutput[i].a * weight[0] + calcOutput[i].b * weight[1] << endl;
	}

	return 0;
}

