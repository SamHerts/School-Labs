//CSIS11-Lab9-Battleship-New
//CSIS 111-001
//Permission from Professor to use advanced knowledge of C++ including classes.

//Include statements
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <time.h>
#include "Tile.h"

using namespace std;

//Function prototypes
int inputFail(int, int, char);

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Extra Credit" << endl << endl;

	//Variable declarations
	srand(time(NULL));//seed random function

	int rando;//store random number
	ofstream output;
	string input;
	int x, y;	//coordinates from user
	const int PERCENT_SHIP = 1; //percent of objects that become ships
	const int MAX_X = 25;//max rows
	const int MAX_Y = 25;//max cols

	Tile* Map[MAX_X][MAX_Y];//array of ship pointers to store actual ships
	Tile* Window[MAX_X][MAX_Y];//array of ship pointers to show to user

	output.exceptions(ifstream::failbit | ifstream::badbit); //Setting up exceptions
	try {
		output.open("Map.txt", std::ofstream::out | std::ofstream::trunc);//Open file to output mode, and erase anything inside
		output.close();
	}
	catch (const ifstream::failure& e) {
		cout << "Error writing to file";
		system("pause");
		return 1;
	}

	output.open("Map.txt");//Outputting to map file is for user only, to cheat if they desire
	Tile dummy(-1, -1, false);// create a dummy object to keep track of static variables and allow for setting values

							  //2D array to fill map with either ships or water
	for (int row = 0; row < MAX_X; row++) {
		for (int col = 0; col < MAX_Y; col++) {
			Window[row][col] = &dummy;//set user view to all water
			rando = 1 + rand() % 100;//get a random number
			if (rando <= PERCENT_SHIP) {//set object to either ship or water
				Map[row][col] = new Tile(row, col, true);//is ship
			}
			else {
				Map[row][col] = new Tile(row, col, false);//is water
			}
			output << Map[row][col]; // send objects to file -- overloaded << operator
		}
		output << endl;
	}
	output.close();

	while (dummy.shipsLeft() > 0) {	//if there are still ships not sunk, keep playing	

		for (int row = 0; row < MAX_X; row++) {//output water map to user
			for (int col = 0; col < MAX_Y; col++) {
				cout << Window[row][col];
			}
			cout << endl;
		}

		cout << "Lets play BattleShip!" << endl << endl;
		x = inputFail(-1, MAX_X - 1, 'X');	//get values from user	
		y = inputFail(-1, MAX_Y - 1, 'Y');
		system("CLS");//clear screen so new ships can be shown to user, either hit or miss

		Map[x][y]->HIT();//test for hit or miss

		Window[x][y] = Map[x][y]; //update user view so they can see real results -- overloaded = operator	
	}
	cout << "All ships have been sunk! You win!" << endl;//user wins, they receive nothing
														 //Closing program statements
	system("pause");
	return 0;
}

//Function definitions
int inputFail(int Left, int Right, char Middle) {//test for invalid input
	string input;
	int result = -1;
	bool test = false;

	cout << "Enter " << Middle << " coordinate between " << Left + 1 << " and " << Right << ": ";
	cin >> input;//get actual user input



	for (int i = 0; i < input.length(); i++) {//for each character in string
		if (isdigit(input[i])) {//If it is a digit, continue
			test = true;
		}
		else {//else break from loop and return sentinel
			test = false;
			break;
		}
	}
	if (test == true) {//if input is valid convert string to integer
		result = atoi(input.c_str());
	}
	else {//if input is invalid return sentinel
		result = -1;
	}

	while (result == Left || result > Right) {//if negative or out of bounds of array, retry
		cout << "improper input." << endl;
		cout << "Enter " << Middle << " coordinate between " << Left + 1 << " and " << Right << ": ";
		cin >> input;
		result = inputFail(Left, Right, Middle);
	}

	return result; //return coordinate value between 0 and max range
}
