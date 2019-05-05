#include "myDict.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
	
	ifstream input;
	input.open("int-string.txt");//Text file of KVpairs
	
	int key;
	string value;
	BDictionary<int, string> newDict;//Object of BDictionary
	//KVpair<int, string> otherKV;

	while (input >> key >> value) {	//Get KVpairs from file
		KVpair<int, string> myKV = { key, value };
		if (newDict.size() < 10) {
			newDict.insert(key, value);//insert kv into dictionary if not greater than the max allowed size
		}		
	}
	input.close();

	key = 5;

	if (newDict.find(key, value)) { //find function
		cout << "Found in newDict " << key << " " << value << endl;
	}
	
	if (newDict.remove(key, value)) { // remove function
		cout << "Removed from  newDict " << key << " " << value << endl;
	}
	if (newDict.removeAny(value)) { // remove any function
		cout << "removed random in newDict " << value << endl;
	}
	newDict.clear(); // clear function

	BDictionary<string, int> otherDict; // object of BDictionary with string, int type
	input.open("string-int.txt");

	string newkey;
	int newvalue;

	while (input >> newkey >> newvalue) { //get kvpairs from file
		KVpair<string, int> myKV = { newkey, newvalue };
		otherDict.insert(newkey, newvalue); // insert function
	}
	input.close();

	newkey = "Noah";
	otherDict.find(newkey, newvalue); // find function
	cout << "Found in other Dict " << newkey << " " << newvalue << endl << endl;
	
	system("pause");
	return 0;
}