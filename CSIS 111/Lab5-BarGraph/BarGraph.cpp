//CSIS111-Lab5-BarGraph-New
//CSIS 111-001
//Permission at beginning of semester to use advance c++ knowledge

//Include statements
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 5" << endl << endl;

	//Variable declarations
	ifstream fin;
	ofstream fout;
	int temp;
	float nStar = 3;

	//Program logic
	fin.open("InputTemperatures.txt");
	fout.open("OutputTemperatures.txt");
	if (!fin) {
		cout << "File Error.\n";
		system("pause");
		return 1;
	}
	//Display Axis Title
	cout << setw(6) << -30 << setw(9) << 0 << setw(11) << 30 <<setw(10) << 60 <<setw(10) << 90 << setw(10) << 120 << endl;
	fout << setw(6) << -30 << setw(9) << 0 << setw(11) << 30 << setw(10) << 60 << setw(10) << 90 << setw(10) << 120 << endl;

	

	while (!fin.eof()){//While not the end of the file
		fin >> temp; // get temperature
		cout << setw(3) << temp; // output the temperature
		fout << setw(3) << temp;
		if (temp >= 0) {
			cout << setw(12) << '|';// greater than 0, so blank spaces first
			fout << setw(12) << '|';
			if (temp > 0) { // nonzero so stars next
				for (int t = 0; t <= (temp / nStar); t++) {//Output one asterisk per 3 degrees				
					cout << "*";
					fout << "*";
				}
			}
		}
		else {
			for (int t = 0; t < 10 - abs(temp) / nStar + 1; t++) {
				//Output one space per asterisk minus 10
				cout << " ";
				fout << " ";
			}
			for (int t = 0; t <= ((abs(temp)) / nStar) - 1; t++) {
				//Output one asterisk per 3 degrees
				cout << "*";
				fout << "*";
			}
			cout << "|";
			fout << "|";
		}
		cout << endl;	
		fout << endl;

	}
	cout << setw(6) << -30 << setw(9) << 0 << setw(11) << 30 << setw(10) << 60 << setw(10) << 90 << setw(10) << 120 << endl;
	fout << setw(6) << -30 << setw(9) << 0 << setw(11) << 30 << setw(10) << 60 << setw(10) << 90 << setw(10) << 120 << endl;

	fout.close();
	fin.close();

	system("pause");
	return 0;
}
