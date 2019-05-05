//<CSIS112Lab1-Ceramic> -- Ceramic Tile Lab 1
//CSIS 112-001



//Include statements
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

//Global declarations: Constants and type definitions only -- no variables
const int boxNumber = 20; //Number of tiles per box

//Function prototypes
int getRoomSize(int, string); 
void returnResults(int, int);
int tilesNeeded(int, int);
int inputFail(string);

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 1" << endl << endl;
	
	//Variable declarations	
	string tileSizeF, roomsF;
	int tileSize, rooms;
	int tileTotal{ 0 }, boxSum{ 0 }, tileSum{ 0 }, widthTotal{ 0 }, lengthTotal{ 0 }; //This new way of initializing is wonderful, thank you for teaching it!
	

	//Program logic

	cout << "Enter size of tile in inches: " << endl; //Get tile size from user
	cin >> tileSizeF;
	tileSize = inputFail(tileSizeF);
	while (tileSize <= 0 ){ //Test that tile size is valid and non zero

		cout << "Enter size of tile in inches (Input should be an integer > 0): " << endl;
		cin >> tileSizeF;
		tileSize = inputFail(tileSizeF);
	}	

	cout << "Enter number of rooms: " << endl; //Get number of rooms from user
	cin >> roomsF;
	rooms = inputFail(roomsF);
	while (rooms <= 0){		//Test that room size is valid and non zero
		cout << "Enter number of rooms (Input should be an integer > 0): " << endl;
		cin >> roomsF;
		rooms = inputFail(roomsF);
	}	

	for (int i = 0; i < rooms; i++){ //Repeat for each room

		widthTotal = getRoomSize(i, "Width");
		
		lengthTotal = getRoomSize(i, "Length");
		
		tileTotal = ((tilesNeeded(widthTotal, tileSize))*(tilesNeeded(lengthTotal, tileSize)));		//get the total number of tiles needed per room by getting number of tiles needed per side of room then multiplying for total tiles needed per room	

		cout << "\nRoom requires " << tileTotal << " tiles.\n";		 
		tileSum += tileTotal;//save answers for final total
	}

	boxSum = tileSum / boxNumber; // Get the number of boxes needed, rounding up when necessary
	if (tileSum%boxNumber != 0){
		boxSum++;
	}

	returnResults(tileSum, boxSum); //Returns Results

	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions

//Gets room size from user
int getRoomSize(int rooms, string side){
	string Flengthf, Flengthi;
	int lengthf, lengthi;

	//get room width and check for correct input
	cout << "Enter room " << rooms+ 1 << " " << side << " (feet and inches, separated by a space): " << endl; 
	cin >> Flengthf >> Flengthi;
	lengthf = inputFail(Flengthf);
	lengthi = inputFail(Flengthi);
	while (lengthf <= 0 || lengthi < 0 || lengthi > 11){//Test lengths in feet and inches for valid input, non zero for feet, and non negative for inches
		cout << "Enter room " << rooms + 1 << " " << side << " (feet > 0 and inches < 11, separated by a space): " << endl;
		cin >> Flengthf >> Flengthi;
		lengthf = inputFail(Flengthf);
		lengthi = inputFail(Flengthi);
	}

	return ((lengthf * 12) + lengthi);//convert feet to inches and return total inches of room side
}

void returnResults(int tileSum, int boxSum){// Outputs results of calculations
	cout << "\nTotal tiles required is " << tileSum << " tiles." << endl; //output total results for entire building
	cout << "Number of boxes needed is " << boxSum << "." << endl;
	cout << "There will be " << (boxSum*boxNumber) - tileSum << " extra tiles." << endl; //Calculate and return extra tiles
}

int tilesNeeded(int length, int tileSize){//Returns the number of tiles needed per side of room
	int needed;
	needed = length / tileSize;
	if (length%tileSize != 0){
		needed++;
	}
	return needed;
}

int inputFail(string input){//test for invalid input
	int result = -1;
	
	bool test = false;
	for (int i = 0; i < input.length(); i++){//for each character in string
		if (isdigit(input[i]) ){//If it is a digit, continue
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
