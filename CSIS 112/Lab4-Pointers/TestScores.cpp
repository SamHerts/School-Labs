#include "TestScores.h"
#include <vector>
#include <iostream>

using namespace std;

TestScores::TestScores()//Constructor 
{
	//left this in because future use might need it
}

TestScores::~TestScores()//Destructor
{
	//left this in because future use might need it
}
void TestScores::addScore(int* addPtr){ //adds score to vector, error checking is done in main

	scores.push_back(*addPtr);

}


void TestScores::sortVector(){ //simple bubblesort.
	int temp;

	for (int i = 0; i < scores.size(); ++i){ //run through array
		for (int j = 0; j < (scores.size() - 1) - i; ++j){
			if (scores.at(j) > scores.at(j + 1))
			{
				temp = scores.at(j); //swap first with temp
				scores.at(j) = scores.at(j + 1); //swap second with first
				scores.at(j + 1) = temp; //swap temp with second
			}
		}
	}	
}
float TestScores::computeAverage(){
	float sum{ 0 };
	for (int i : scores){ //sum all elements
		sum += i;
	}
	return sum / (scores.size()-1); //return sum divided by size of array-1
}
void TestScores::Print(){
	for (int i : scores){ //outputs all grades
		cout << i << " ";
	}
	cout << endl;
}
void TestScores::Clear(){	
		scores.clear();	
}



	
