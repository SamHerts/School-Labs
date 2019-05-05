//CSIS111-Lab3-TempData-New
//CSIS 111-001
//Include statements
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 3" << endl << endl;
	//Variable declarations	
	ofstream output;
	float input, last{ 0 }, avg{ 0 };
	const int nTEMPS{ 12 };

	//Program logic
	output.open("tempdata.dat");

	output << fixed << showpoint << setprecision(1);//Set up tags for outputting

	for (int i = 0; i < nTEMPS; i++) {//Ask for nTemps temperatures
		cout << "Enter temperature " << i + 1 << ": ";
		cin >> input;
		output << setw(6) << input;

		(i != 0) ? (output << setw(10) << input - last << endl) : (output << endl);//if not the first temperature, output the difference between this and the last
		last = input;
		avg += input;
	}
	cout << "Average temperature is: " << avg / nTEMPS << " degrees." << endl;//compute average
	output.close();

	//Closing program statements
	system("pause");
	return 0;
}
