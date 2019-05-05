#pragma once
#include <iostream>
#include <string>

class Mammal
{
public:
	Mammal(); //default 
	Mammal(int); //set weight constructor

	virtual void Speak();//virtual as a generic mammal cannot speak
	void setWeight(double);
	double getWeight();
	virtual void setName(std::string) = 0; //virtual and pure as a generic mammal cannot have a name
	virtual std::string getName() = 0;
	virtual std::string getType() = 0;

private:
	int weight;	
};


