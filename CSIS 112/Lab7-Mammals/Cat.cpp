#include "Cat.h"

Cat::Cat(int lb) : Mammal(lb)
{
	std::cout << "Invoking Cat constructor\n";
}
std::string Cat::getName(){
	return Name;
}
void Cat::setName(std::string input){
	Name = input;
}
void Cat::Speak(){
	Mammal::Speak();
	std::cout << "Meow!\n";
}
std::string Cat::getType(){
	return Type;
}