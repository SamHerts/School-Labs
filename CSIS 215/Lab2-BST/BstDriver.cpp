#include <iostream>
#include <string>
#include "BST.h"

using namespace std;

int main() {

	BST<int, string> myBST;	

	myBST.insert(77, "seventy - seven");
	myBST.insert(70, "seventy");
	myBST.insert(75, "seventy-five");
	myBST.insert(66, "sixty-six");
	myBST.insert(79, "seventy-nine");
	myBST.insert(68, "sixty-eight");
	myBST.insert(67, "sixty-seven");
	myBST.insert(69, "sixty-nine");
	myBST.insert(90, "ninety");
	myBST.insert(85, "eighty-five");
	myBST.insert(83, "eighty-three");
	myBST.insert(87, "eighty-seven");
	myBST.insert(65, "sixty - five");
	
	myBST.print();
	myBST.printInOrder();	
	myBST.printReverse();

	system("pause");
	return 0;
}

