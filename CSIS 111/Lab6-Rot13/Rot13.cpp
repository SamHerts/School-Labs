//CSIS111-Lab6-Rot13-New
//CSIS 111-001

//Include Statements
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//Initialization of functions
void rot13(string&);
void WriteTranslatedChar(string, ostream&);

int main()
{
	cout << "Samuel Herts -- Lab 6" << endl << endl;

	ifstream input;
	ofstream output;

	//Get name of file to be converted to Rot13
	string inputFile, outputFile, str;
	cout << "Name of File to be Encrypted: ";
	cin >> inputFile;

	input.open(inputFile); //Open user's file to be encrypted
	if (!input) {
		cout << "Error opening input file." << endl;
		system("pause");
		return 1;//Error opening input file
	}
	output.open("Rot13.txt"); // Open file for encryption output
	if (!output) {
		cout << "Error opening output file." << endl;
		system("pause");
		return 2;//error opening output file (Most likely will not happen, as C++ will just create the file)
	}


	while (!input.eof()) { //Read file till the file ends
		getline(input, str); //Read a line from the file
		rot13(str); //Convert line of file to Rot13
		WriteTranslatedChar(str, output); //Output the translated line 
		WriteTranslatedChar(str, cout);//Output translated line to console
	}
	cout << "Encryption successful" << endl; //Assure user that they didn't screw everything up	

	system("pause");
	return 0;
}
void rot13(string& str) { //Algorithm for encrypting using Rot13

	for (int i = 0; i < str.size(); i++) { //Read string till the end
		if ((str[i] >= 65 && str[i] <= 77) || (str[i] >= 97 && str[i] <= 109)) { //Convert A-M using ASCII code
			str[i] += 13;
		}
		else if ((str[i] >= 78 && str[i] <= 90) || (str[i] >= 110 && str[i] <= 122)) { //Convert N-Z letters using ASCII
			str[i] -= 13;
		}
	}
}

void WriteTranslatedChar(string word, ostream& output) { // Output translated line to predetermined file

	for (int i = 0; i < word.size(); i++) { //Output string letter by letter
		output << word[i];
	}
	output << endl;
}
