//CSIS111-Lab2-Permutations-New
//CSIS 111-001
//Samuel Herts
//Sources cplusplus.com for cmath defines.
//Granted permission at beginning of semester to use advanced knowledge of c++.


//header files
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>


using namespace std;

//initializing functions of Factorials
double sFactorial(int n) { return (exp(-n))*(pow(n, n))*(sqrt(M_PI * 2 * n)); }; // Inline Stirling calculation
double rFactorial(int ); //True recursive calculation
double cFactorial(int ); //Looped Calculation;

int main(){
	int students{ 12 };
	int groups{ 3 };	
	double permutations;	

	//Performing the Stirling calculation
	permutations = sFactorial(students) / (sFactorial(groups) * sFactorial(students - groups));

	//Printing to screen the input and output of the functions
	cout << "The number of permutations of " << students << " students into " << groups << " groups is " << endl
		<< permutations << " using the Stirling Approximation" << endl;	

	//Performing the Recursive calculation
	permutations = rFactorial(students) / (rFactorial(groups) * rFactorial(students - groups));

	//Printing to screen the input and output of the functions
	cout << "The number of permutations of " << students << " students into " << groups << " groups is " << endl
		<< permutations << " using the Recursive method" << endl;
	//Performing the Recursive calculation
	permutations = cFactorial(students) / (cFactorial(groups) * cFactorial(students - groups));

	cout << "The number of permutations of " << students << " students into " << groups << " groups is " << endl
		<< permutations << " using the Looped Computation method" << endl;

	//Closing the program
	system("pause");
	return 0;
}

//Factorial using true recursion
double rFactorial(int x) {
	double output;
	if (x > 1) {
		output = x * rFactorial(x - 1);
	}
	else {
		output = 1;
	}
	return output;
}
//Factorial using  multiplication
double cFactorial(int n){
	//Factoring starting at one and continuing to n
	double factorial = 1;
	for (int i = 1; i <= n; i++){
		factorial *= i;
	}
	return factorial;
}
