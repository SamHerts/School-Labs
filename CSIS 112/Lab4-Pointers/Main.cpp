//<Lab4-Pointers> -- brief statement as to the file’s purpose
//CSIS 112-001
//<Sources if necessary>


//Include statements
#include <iostream>
#include <string>
#include "TestScores.h"
#include <vector>
#include <fstream>

using namespace std;

//Global declarations: Constants and type definitions only -- no variables

//Function prototypes
void Menu();
int inputFail(string);

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 4" << endl << endl;

	//Variable declarations
	int choice{ 0 }; //for user input
	int newResult{ 0 }; //for string to int conversion
	int result{ 0 }; //for string to int conversion
	string grade{ 0 }; //for file input
	string filename{ 0 }; //name of file from user
	ifstream input;

	//Program logic
	TestScores newScores; //creates new object
	//Menu

	do
	{
		Menu();

		cin >> choice;
		

		switch (choice)
		{		
		case 1:	//read from file
			cout << "enter file name: ";
			cin.ignore(10000, '\n');			
			getline(cin, filename);				
			input.open(filename);

			if (input.is_open()){ //ensure file opens
				while (input >> grade){
					result = inputFail(grade); //takes in string, checks for positive, returns -1 if negative
					newScores.addScore(&result);
					if (result == -1){
						cout << "File contained negative grade. -1 has replaced this in the score list."; //add the negative, but tell user that they messed up
					}						
				}
				input.close();
				cout << "Input from file complete." << endl;
			}
			else{
				cout << "Unable to open file." << endl;
			}
			break;	
		case 2:		//get grade from user
			cout << "Enter a grade: ";
			cin >> grade;
			while (inputFail(grade) < 0){ //ensure grades are positive
				cout << "Input must be a positive integer. Please try again: ";
				cin >> grade;
				cout << endl;
			}
			newResult = inputFail(grade);
			newScores.addScore(&newResult);

			cout << "Grade successfully added." << endl;
			break;	
		case 3://sort grades using bubblesort because its an easy implementation and trying to implement quicksort ended up being very difficult
			cout << "Sorting grades..." << endl;
			newScores.sortVector();
			cout << "Sorting complete." << endl;
			break;
		case 4: //calculate average
			cout << "the average grade is: " << newScores.computeAverage();
			break;
		case 5: //print to screen
			cout << "The scores are:" << endl;
			newScores.Print();
			break;
		case 6: //added an extra clear function because I felt it was a useful thing to have.
			cout << "Clearing scores..." << endl;
			newScores.Clear();
			cout << "Scores have been cleared." << endl;
			break;
		case 7: //user wants to quit
			cout << "Thank you for using this program." << endl;
			break;		
		default:
			cout << "You have not entered a valid choice.  Please try again." << endl;
			break;
		}

		cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n');

	} while (choice >0 && choice != 7);

	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions
void Menu(){ //outputs the menu so its not clogging up main
	cout << endl << endl << endl;

	cout << "Enter a selection:" << endl;
	cout << "1. Read scores from file" << endl;
	cout << "2. Add a score from keyboard" << endl;
	cout << "3. Sort the scores" << endl;
	cout << "4. Compute the average score" << endl;
	cout << "5. Print scores to the screen" << endl;
	cout << "6. Clear scores" << endl;
	cout << "7. Exit program" << endl << endl;

	cout << "Enter Selection: ";
}
//using parser from Ceramics lab. created by me. its not plagurism if its your own work...right?
int inputFail(string input){//test for invalid input
	int result = -1;

	bool test = false;
	for (int i = 0; i < input.length(); i++){//for each character in string
		if (isdigit(input[i])){//If it is a digit, continue
			test = true;
		}
		else{//else break from loop and return sentinel
			test = false;
			break;
		}
	}
	if (test == true){//if input is valid convert string to integer
		result = atoi(input.c_str());
	}
	else{//if input is invalid return sentinel
		result = -1;
	}

	return result;
}




	

		
	
