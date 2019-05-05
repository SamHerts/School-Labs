#include "Horse.h"

Horse::Horse(int lb) : Mammal(lb)
{
	std::cout << "Invoking Horse constructor\n";
}
std::string Horse::getName(){
	return Name;
}
void Horse::setName(std::string input){
	Name = input;
}
void Horse::Speak(){
	Mammal::Speak();
	std::cout << "I'm Mr. Ed!\n";
}
std::string Horse::getType(){
	return Type;
}