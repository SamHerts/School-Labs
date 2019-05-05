#pragma once
#include "Mammal.h"
#include <string>

class Pig : public Mammal
{
public:	
	Pig(int);	
	std::string getName();
	void setName(std::string);
	void Speak();
	std::string getType();
private:
	std::string Name;
	std::string Type = "Pig";
};