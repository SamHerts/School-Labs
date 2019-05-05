//<Mammals> -- brief statement as to the file’s purpose
//CSIS 112-001
//Include statements
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include "Pig.h"
#include "Horse.h"
#include "Dog.h"
#include "Cat.h"
using namespace std;
//Function prototypes
void menu();

int main()
{
	srand(100);
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 7" << endl << endl;
	//Variable declarations

	int choice;	
	string input;
	vector<Mammal *> List; // List of Mammals created

	//Program logic
	do{
		system("cls");
		menu();
		cin >> choice;
		cin.clear();
		cin.ignore(100000, '\n');
		
		switch (choice){
		case 1:
		{
			Pig* p = new Pig(rand() % 150); //Create pig with random weight
			List.push_back(p); // add pig to vector
			break;
		}
		case 2:
		{
			Horse* h = new Horse(rand() % 150); //Create horse with random weight
			List.push_back(h); // add horse to vector

			break;
		}
		case 3:
		{
			Dog* d = new Dog(rand() % 150); // create dog with random weight
			List.push_back(d); // add dog to vector
			break;
		}
		case 4:
		{
			Cat* c = new Cat(rand() % 150); //create cat with random weight
			List.push_back(c); // add cat to vector
			break;
		}
		default:
			cout << "You have not entered a valid choice.  Please try again." << endl;
			break;
		}
		//set name for most recently added mammal
		if (List.size() > 0){ // ensure vector is not zero so vector.back() works
			cout << "Enter Name of creature: ";
			getline(cin, input);
			Mammal *point = List.back(); //access most recent mammal
			(*point).setName(input); // set name
		}
		system("pause");
	} while (List.size() < 5);

	cout << endl << endl;
	for (Mammal *i : List){ //Make all creatures in vector speak their sound
		cout << "The " << (*i).getType() << " named " << (*i).getName() << " Says ";
		(*i).Speak();
	}
	for (Mammal *i : List){ //delete all creatures
		delete i;
	}
	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions
void menu(){ //Outputs Menu
	cout << "Samuel Herts -- Lab 7" << endl << endl;
	cout << "Choose a mammal from the following list.";
	cout << endl << endl << endl;
	cout << "1. Pig" << endl;
	cout << "2. Horse" << endl;
	cout << "3. Dog" << endl;
	cout << "4. Cat" << endl;	
	cout << "Enter Selection: ";
}