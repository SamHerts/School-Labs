#pragma once
#include "Mammal.h"
#include <string>

class Horse : public Mammal
{
public:	
	Horse(int);	
	void setName(std::string);
	std::string getName();
	void Speak();
	std::string getType();

private:
	std::string Name;
	std::string Type = "Horse";
};