//CSIS111-Lab4-ComputeGrades-New
//CSIS 111-001

//Include statements
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 4" << endl << endl;

	//Variable declarations
	ifstream input;
	string Name, output;
	double gotten, possible, total;
	
	//Program logic
	input.open("GradesList.txt");
	if (!input) {
		cout << "File Error.\n";
		system("pause");
		return 1;
	}

	cout << fixed << showpoint << setprecision(5);
	while (!input.eof()) {
		input >> Name >> gotten >> possible;
		total = gotten / possible;
		if (total > .90) {
			output = " Excellent";
		}
		else if (total > .8) {
			output = " Well Done";
		}
		else if (total > .7) {
			output = " Good";
		}
		else if (total >= .6) {
			output = " Needs Improvement";
		}
		else {
			output = " Fail";
		}
		cout << left <<setw(8) << Name << " " << right << setw(4) << (int)ceil(total*100) << "% " << total << output << endl;
	}
	input.close();

	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions
