#include <string>
#include <iostream>
#include <fstream>
#include "SOL.h"

using namespace std;

void TestOne();
void TestTwo();

int main() {
	TestOne();//self organizing char lists based on alphabet
	TestTwo(); //self organizing string lists based on file

	system("Pause");
	return 0;
}

void TestOne() {
	//Test 1 with Chars
	SOL<char> CcharList(0);
	SOL<char> McharList(1);
	SOL<char> TcharList(2);
	char myArray[] = { 'A','B','C','D','E','F', 'G', 'H'};

	//A B C D E F G H
	for (char i : myArray) {
		CcharList.add(i);
		McharList.add(i);
		TcharList.add(i);
	}	
	
	char myOther[] = { 'F', 'D', 'F', 'G','E','G','F','A','D','F','G','E','H','I'};
	//F D F G E G F A D F G E H I
	for (char i : myOther) {
		
		CcharList.find(i);
		McharList.find(i);
		TcharList.find(i);
	}

	cout << "Count Heuristic: " << endl;
	cout << "Size of List: " << CcharList.size() << endl;
	cout << "Number of Compares: " << CcharList.getCompares() << endl;
	CcharList.printlist();
	cout << endl << endl;

	cout << "Move to Front Heuristic: " << endl;
	cout << "Size of List: " << McharList.size() << endl;
	cout << "Number of Compares: " << McharList.getCompares() << endl;
	McharList.printlist();
	cout << endl << endl;

	cout << "Transpose Heuristic: " << endl;
	cout << "Size of List: " << TcharList.size() << endl;
	cout << "Number of Compares: " << TcharList.getCompares() << endl;
	TcharList.printlist();
	cout << endl << endl << endl;

}

void TestTwo() {
	SOL<string> CcharList(0);
	SOL<string> McharList(1);
	SOL<string> TcharList(2);

	ifstream input;
	input.open("test.txt");
	string word;

	while (input >> word) {
		CcharList.find(word);
		McharList.find(word);
		TcharList.find(word);
	}
	cout << "Count Heuristic: " << endl;
	cout << "Size of List: " << CcharList.size() << endl;
	cout << "Number of Compares: " << CcharList.getCompares() << endl;
	CcharList.printlist(10);
	cout << endl << endl;

	cout << "Move to Front Heuristic: " << endl;
	cout << "Size of List: " << McharList.size() << endl;
	cout << "Number of Compares: " << McharList.getCompares() << endl;
	McharList.printlist(10);
	cout << endl << endl;

	cout << "Transpose Heuristic: " << endl;
	cout << "Size of List: " << TcharList.size() << endl;
	cout << "Number of Compares: " << TcharList.getCompares() << endl;
	TcharList.printlist(10);
	cout << endl << endl;
}