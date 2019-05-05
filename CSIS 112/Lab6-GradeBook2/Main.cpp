//<Lab6> -- Lab 6
//CSIS 112-001
//Include statements
#include <iostream>
#include <string>
#include "GradeBook.h"

using namespace std;

//Global declarations: Constants and type definitions only -- no variables

//Function prototypes
void menu();

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Your name -- Lab 6" << endl << endl;

	//Variable declarations
	
	char choice;
	string input;
	

	//Program logic
	cout << "Enter a title for your Gradebook: " << endl;
	getline(cin, input);
	GradeBook gradeBook(input);	

	do{
		system("cls");
		cout << "\t\t\tYou are viewing gradebook: " << gradeBook.getTitle() << endl;
		menu();
		cin >> choice;
		cin.ignore();
		switch (choice){
		case 'A':
		case 'a': gradeBook.addStudent();			
			break;
		case 'L':
		case 'l':
			if (gradeBook.size() > 0){
				gradeBook.listStudents();
			}
			else{
				cout << "GradeBook is empty\n";
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
		system("pause");
	} while (choice != 'Q' && choice != 'q');

	//Closing program statements
	//system("pause");
	return 0;
}

//Function definitions
void menu(){ //Outputs Menu
	
	cout << endl << endl << endl;
	cout << "Enter a selection:" << endl;
	cout << "'L' - LIST the student's data" << endl;
	cout << "'A' - ADD a new student and his/her grades" << endl;

	cout << "Q - Quit" << endl << endl;
	cout << "Enter Selection: ";
}