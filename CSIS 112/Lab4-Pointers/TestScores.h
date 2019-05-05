#pragma once

#include <string>
#include <iostream>
#include <vector>

class TestScores
{
public:
	TestScores();//Constructor
	~TestScores();//Destructor
	void addScore(int*); //adds a score to the end of vector
	void sortVector(); //sorts vector with bubblesort
	float computeAverage(); //returns average of vector
	void Print(); //prints data
	void Clear(); //Extra function
	

private:
	std::vector<int> scores;

};


