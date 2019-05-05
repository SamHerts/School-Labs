//CSIS11-Lab7-Fibonacci-New
//CSIS 111-001
//I added two other methods of calculating the nth fib number, as well as a way to time the functions to see which is fastest, as well as an output to file so I can graph it in excel.

//Include statements
#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

//Function prototypes
long lFib(int); // Calculate nth fib using a loop
long rFib(long);// Calculate nth fib using recursion
long eFib(int); // Calculate nth fib using equation


int main()
{
	//In cout statement below substitute your name and lab number
	cout << "Your name -- Lab Number" << endl << endl;

	//Variable declarations
	int input; //get input from using
	double duration; // used for calculating time
	ofstream output;
	output.open("Data.txt");

	//Program logic
	while (true) {
		cout << "Enter a number: ";
		cin >> input;
		output << input << "\t";
		
		high_resolution_clock::time_point t1 = high_resolution_clock::now();//start clock

		cout << "The " << input << "th Fibonacci number using a loop is " << lFib(input) << endl;//find nth fib

		high_resolution_clock::time_point t2 = high_resolution_clock::now(); //end clock
		 duration = duration_cast<microseconds>(t2 - t1).count()*0.000001;//calculate time in seconds
		cout << "This method took " << duration << " seconds." << endl;//output time taken for function
		output << duration << "\t";

		t1 = high_resolution_clock::now();//start clock

		cout << "The " << input << "th Fibonacci number using equation is " << eFib(input) << endl;

		t2 = high_resolution_clock::now();//end clock
		duration = duration_cast<microseconds>(t2 - t1).count()*0.000001;//calculate time in seconds
		cout << "This method took " << duration << " seconds." << endl;// output time take for function
		output << duration << "\t";

		t1 = high_resolution_clock::now();//start clock

		cout << "The " << input << "th Fibonacci number using recursion is " << rFib(input) << endl;// find nth fib

		t2 = high_resolution_clock::now();// end clock
		duration = duration_cast<microseconds>(t2 - t1).count()*0.000001;// calculate time take for function
		cout << "This method took " << duration << " seconds." << endl;//output time taken
		output << duration << endl;

		

	}
	output.close();

	//Closing program statements
	system("pause");
	return 0;
}

//Function definitions8

long lFib(int n) {
	//starts with 0 and 1
	long number1 = 0;
	long number2 = 1;
	long answer = 0;

	if (n == 0 || n == 1) { //Sets case of 1 or 0th fibonacci number
		return n;
	}
	else {
		for (int i = 0; i < n; i++) {
			//loops n times and calculates nth fibonacci number
			number1 = number2;
			number2 = answer;
			answer = number1 + number2;
		}
		//returns nth fibonacci
		return answer;
	}
}

long rFib(long input) {
	long output;

	if (input == 0 || input == 1) {
		output = input;//if n is less than 2
	}
	else {
		output = rFib(input - 1) + rFib(input - 2);//recursion
	}
	return output;
}

long eFib(int n) {
	double answer;
	//equation from wikipedia
	const double SQRT_FIVE = sqrt(5);//square root of 5 constant
	const double PHI = (1 + SQRT_FIVE) / 2; //golden ration constant
	const double NEG_PHI = (1 - SQRT_FIVE) / 2; //negative golden ratio constant
	answer = pow(PHI, n) - pow(NEG_PHI, n);//first half of equation
	answer /= SQRT_FIVE;//second half of equation
	return answer;
}
