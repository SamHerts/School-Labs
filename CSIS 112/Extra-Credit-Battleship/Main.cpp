//<File name> -- brief statement as to the file’s purpose
//CSIS 112-<Section Number>
//<Sources if necessary>


//Include statements
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <time.h>
#include "Ship.h"
#include "Water.h"

using namespace std;

//Global declarations: Constants and type definitions only -- no variables

//Function prototypes
int inputFail(string);

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Extra Credit" << endl << endl;

	//Variable declarations
	srand(time(NULL));
	Tile* Map[25][25];
	int rando;
	ofstream output;
	string input;
	int x;
	int y;


	for (int i = 0; i < 25; i++){
		for (int j = 0; j < 25; j++){
			rando = 1 + rand() % 100;
			if (rando < 8){
				*(*(Map + i)+j) = new Ship(i, j);
			}
			else{
				*(*(Map + i) + j) = new Water(i, j);
			}
		}
	}
	output.exceptions(ifstream::failbit | ifstream::badbit);
	
	try{
		output.open("Map.txt");
		for (int i = 0; i < 25; i++){
			for (int j = 0; j < 25; j++){
				output << *(*(Map + i) + j);
			}
			output << endl;
		}
	}
	catch (const ifstream::failure& e) {
		cout << "Error writing to file";
	}
	output.close();

	Ship dummy(100, 100);

	while (dummy.shipsLeft() - 1 > 0){
		cout << "Lets play BattleShip!" << endl << endl;
		cout << "Enter an X coordinate between 0 and 24: ";
		cin >> input;
		x = inputFail(input);
		
		while (x == -1 || x > 24){
			cout << "improper input." << endl;
			cout << "Enter an X coordinate between 0 and 24: ";
			cin >> input;
			x = inputFail(input);
			
		}

		cout << "Enter a Y coordinate between 0 and 24: ";
		cin >> input;
		y = inputFail(input);
		while (y == -1 || y > 24){
			cout << "improper input." << endl;
			cout << "Enter a y coordinate between 0 and 24: ";
			cin >> input;
			y = inputFail(input);
		}
		(*Map[x][y]).HIT();

	}
	cout << "All ships have been sunk! You win!" << endl;
	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions
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