#include "Pig.h"

Pig::Pig(int lb) : Mammal(lb)
{
	std::cout << "Invoking Pig constructor\n";
}

std::string Pig::getName(){
	return Name;
}
void Pig::setName(std::string input){
	Name = input;
}
void Pig::Speak(){
	Mammal::Speak();
	std::cout << "Oink!\n";
}
std::string Pig::getType(){
	return Type;
}
