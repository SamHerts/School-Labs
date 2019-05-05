//<Rectangle Lab> -- calculate parameters based on user input
//CSIS 112-001

//Include statements
#include <iostream>
#include <string>
#include "Rectangle.h" 

using namespace std;

//Function prototypes
float testInput(string, int counter = 0); //will test the input from user and ensure it fits with guidelines

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 2" << endl << endl;

	//Variable declarations	
	float length{ 0 };
	float width{ 0 };

	//Program logic
	Rectangle newRectangle;//Creates rectangle object

	while (length >= 0){ //run until sentinel

		length = testInput("Length");	//check the input	

		if (length >= 0){ //if user hasn't quit
			width = testInput("Width");	//test the input
			if (width < 0){ //if user tries to input negative, try again
				width = testInput("Width", 1);
			}
			
			newRectangle.setLength(length);//sets up parameters of rectangle object
			newRectangle.setWidth(width);
			newRectangle.showData();
		}
	}

	//Closing program statements
	system("pause");
	return 0;
}


//Function definitions
float testInput(string type, int counter){
	string input;
	float output;
	int deciCounter{ 0 }, negCounter{ 0 }; //keeps track of number of decimals and negative signs
	bool flag{ true }; //if true, input passes, else fail

	if (counter == 0){ //if this is the first test of a given input, ask for input
		cout << "Enter the " << type << " of the rectangle: ";//asks user for input
	}
	else {//if user has failed an input, ask again with warning
		cout << "Enter the " << type << " of the rectangle again, in integer/decimal form: ";//asks user for input
	}

	getline(cin, input); //get the input
	for (int i = 0; i < input.length(); i++){
		
		if (isalpha(input[i]) || iswspace(input[i])){ //check if its a letter or blank space
			flag = false;	//if its a letter, retry input		
		}

		else if (ispunct(input[i])){ //check if its a punctuation mark

			if (input[i] == '.'){ //check if its a decimal
				deciCounter++; //if its a decimal add it to total count of decimals
				if (deciCounter > 1){ // check there is only one decimal
					flag = false; //if there are more than one decimal, retry input
				}
			}
			else if (input[i] == '-'){//check if its negative and only one negative sign
				negCounter++;
				if (negCounter > 1){
					flag = false;
				}
			}
			else{
				flag = false; //if its any other punctuation retry input
			}

		}
	}	
	output = atof(input.c_str()); //copy string to float 
	if (output == 0){ //ensure the input is positive except sentinels
		flag = false;
	}

	if (!flag){
		output = testInput(type, ++counter);//use recursion to ensure the user enters proper input also a good example of the pre and post increment
		
	}

	return output;//return proper float value or sentinel
	
}
