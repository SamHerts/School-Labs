#include "Dog.h"

Dog::Dog(int lb) : Mammal(lb)
{
	std::cout << "Invoking Dog constructor\n";
}
std::string Dog::getName(){
	return Name;
}
void Dog::setName(std::string input){
	Name = input;
}
void Dog::Speak(){
	Mammal::Speak();
	std::cout << "Woof!\n";
}
std::string Dog::getType(){
	return Type;
}