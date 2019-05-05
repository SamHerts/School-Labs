#include "Mammal.h"

Mammal::Mammal()
{
	weight = 0;
	std::cout << "Invoking Mammal default constructor\n";
}
Mammal::Mammal(int way)
{
	weight = way;
	std::cout << "Invoking Mammal explicit constructor\n";
}


void Mammal::Speak(){
	std::cout << "Mammal is speaking\t";
}