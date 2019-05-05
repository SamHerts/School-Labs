#pragma once
#include <string>
#include "Mammal.h"

class Dog : public Mammal
{
public:
	Dog(int);
	void setName(std::string);
	std::string getName();
	void Speak();	
	std::string getType();
private:
	std::string Name;
	std::string Type = "Dog";
};