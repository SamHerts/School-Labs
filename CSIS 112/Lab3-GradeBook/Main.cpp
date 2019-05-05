//<CSIS112-Lab3-GradeBook> -- brief statement as to the file’s purpose
//CSIS 112-001

//Include statements
#include <iostream>
#include <string>
#include "Gradebook.h"
using namespace std;

//Function prototypes
char Menu();

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 3" << endl << endl;

	//Variable declarations

	//Program logic
	string inputName, inputNew;
	cout << "Enter a title for your Gradebook: " << endl; //Get title of gradebook
	getline(cin, inputName);

	Gradebook newBook(inputName); //creates new gradebook

	char choice;
	//Menu	
	do
	{
		newBook.listTitle();
		choice = Menu(); //outputs menu and returns the selection
		
		switch (choice)
		{
			//for upper or lowercase inputs
		case 'A':
		case 'a':
			if (newBook.size() > 20){
				std::cout << "Gradebook is full, please create a new gradebook to add more students.\n";
			}
			else{
				cout << "Enter name of student to add:";
				getline(cin, inputNew);
				cout << "Enter up to 10 grades for the student. Enter -1 to finish:" << endl;
				newBook.addStudent(inputNew); //creates new student
			}
			
			break;
		case 'L':
		case 'l':	
			if (newBook.size() > 0){
				newBook.listStudents(); //lists the students
			}
			else{
				cout << "Gradebook has no students listed.\n";
			}
			break;
		case 'Q':
		case 'q':
			cout << endl << endl << "Thank you for using this program." << endl << endl;
			break;
		default:
			cout << "You have not entered a valid choice.  Please try again." << endl;
			break;
		}
		
		cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		//took this out because it required user to press enter when not wanted.
		
	} while (choice != 'Q' && choice != 'q'); //ensures the program quits after at least once and user enters quit

	//Closing program statements
	std::system("pause");
	return 0;
}

//Function definitions
char Menu(){ //outputs menu
	char choice{ 0 };	
	cout << endl << endl << endl;
	cout << "Enter a selection:" << endl;
	cout << "'L' - LIST the student's data" << endl;
	cout << "'A' - ADD a new student and his/her grades" << endl;

	cout << "Q - Quit" << endl << endl;
	cout << "Enter Selection: ";

	cin >> choice;
	cin.ignore();
	return choice;
};