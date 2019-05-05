//<Lab 5 Splice Array> -- brief statement as to the file’s purpose
//CSIS 112-001
//<Sources if necessary>


//Include statements
#include <iostream>
#include <string>

using namespace std;

//Global declarations: Constants and type definitions only -- no variables

//Function prototypes
int* Splice(int[], int, int[], int, int);
int inputFail(string);
int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Samuel Herts -- Lab 5" << endl << endl;

	//Variable declarations
	srand(100); //seeding
	int arr1Size{ 0 }; //first array
	int arr2Size{ 0 }; // second array
	int splice{ 0 }; //third array
	string input{0};	

	//Program logic
	cout << "Enter size of array 1: ";
	getline(cin, input);
	while (inputFail(input) < 1){ //validate input
		cout << "\nArray must be integer size greater than 0: ";
		getline(cin, input);
	}
	arr1Size = inputFail(input);

	cout << "\nEnter size of array 2: ";
	getline(cin, input);
	while (inputFail(input) < 1){ //validate input
		cout << "\nArray must be integer size greater than 0: ";
		getline(cin, input);
	}
	arr2Size = inputFail(input);
		

	cout << "\nEnter splice position: ";
	getline(cin, input);
	while (inputFail(input) < 1 || inputFail(input) > arr1Size){ //validate input
		cout << "\nSplice must be integer size greater than 0 and less than Array 1 size: ";
		getline(cin, input);
	}
	splice = inputFail(input);

	int *arr1 = new int[arr1Size]; //heap stored array
	int *arr2 = new int[arr2Size]; //heap stored array

	for (int i = 0; i < arr1Size; i++){ //fill array 1 with random ints
		arr1[i] = rand();
		
	}
	for (int j = 0; j < arr2Size; j++){ //fill array 2 with random ints
		arr2[j] = rand();		
	}

	int* spliced = Splice(arr1, arr1Size, arr2, arr2Size, splice); 

	cout << "Array 1:\n";
	for (int i = 0; i < arr1Size; i++){ // output array 1 in 10xn grid
		if ((i % 10 == 0) && i != 0){
			cout << endl;
		}
		cout << arr1[i] << " ";		
	}

	cout << "\nArray 2:\n";
	for (int i = 0; i < arr2Size; i++){ //output array 2 in 10xm grid
		if ((i % 10 == 0) && i != 0){ 
			cout << endl;
		}
		cout << arr2[i] << " ";		
	}
	cout << "\nSpliced Array:\n";
	for (int k = 0; k < (arr1Size+arr2Size); k++){ //output array 3 in 10xk grid
		if ((k % 10 == 0) && k != 0){
			cout << endl;
		}
		cout << spliced[k] << " ";
	}
	cout << endl;
	//Closing program statements

	delete[] arr1;// delete the heap arrays
	delete[] arr2;
	delete[] spliced;//I hope this is how you would delete the third. I dont see why it wouldnt work properly

	system("pause");
	return 0;
}

//Function definitions
int* Splice(int *arr1, int arr1Size, int *arr2, int arr2Size, int spl){
	int *spliceArray = new int[arr1Size + arr2Size];

	for (int i = 0; i < spl; i++){ //splice array 1 up to splice point
		spliceArray[i] = arr1[i];
	}

	for (int i = 0; i < arr2Size; i++){ //splice all of array 2
		spliceArray[spl + i] = arr2[i];
	}

	for (int i = 0; i < (arr1Size-spl); i++){ //splice rest of array 1
		spliceArray[spl + arr2Size + i] = arr1[spl+i];
	}

	return spliceArray; //return pointer to array 3
	//heap array should be deleted from main
}

int inputFail(string input){//Using my own parser
	int result = -1;

	bool test = false;
	for (int i = 0; i < input.length(); i++){//for each character in string
		if (isdigit(input[i])){ //If it is a digit, continue
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
	return result;
}
