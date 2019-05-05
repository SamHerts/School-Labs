#pragma once
#include "Mammal.h"
#include <string>
class Cat : public Mammal
{
public:
	Cat(int);	
	std::string getName();
	void setName(std::string);
	void Speak();
	std::string getType();
private:
	std::string Name;
	std::string Type = "Cat";
};