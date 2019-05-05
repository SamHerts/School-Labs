#include <vector>
#include <numeric>
#include <string>
#include "StudentGradeInfo.h"
using namespace std;

StudentGradeInfo::StudentGradeInfo(string name, int eyedee, string major) : StudentInfo(name, eyedee, major) //gets name, ID, and Major using base class constructor
{
	int input;
	cout << "Enter a grade between 0 and 100, enter negative number to stop adding grades: " << endl;	
	do
	{
		cin >> input;
		if (input >= 0 && input <= 100){ //ensures input is in bound
			testGrades.push_back(input); //applies input to vector
		}
		else if (input > 100) //if input is out of bounds, but not the flag
		{
			cout << "Grade must be between 0 and 100: ";			
		}

	} while (input > -1 && testGrades.size() < 10);//keeps size of vector in bounds and checks for flag
}
void StudentGradeInfo::setGrade(int grade){
	testGrades.push_back(grade);
}
double StudentGradeInfo::getAverage(){
	if (testGrades.size() > 0){//if vector is empty, this function breaks, so check for it
		testAverage = accumulate(testGrades.begin(), testGrades.end(), 0.0) / testGrades.size();//adds the scores then divides by number of elements, gotten from cplusplus reference on summing vector
	}
	else
	{
		testAverage = 0;
	}
	return testAverage;
}
void StudentGradeInfo::printGrades(){
	for (int i : testGrades){ //outputs the grades
		cout << i << " ";
	}
}
bool StudentGradeInfo::operator==(int eyedee)
{
	return (*this).getStudentID() == eyedee; //using equivalence operator, checks ID from object with integer
}
ostream& operator<<(ostream& output, StudentGradeInfo& right){ //outputs the students information in a nice format and allows cascading even though not used.
	cout << "Student Name: " << right.getStudentName() << endl;
	cout << "\tStudent ID: " << right.getStudentID() << endl;
	cout << "\tMajor: " << right.getMajor() << endl;
	cout << "\tGrade Average: " << right.getAverage() << endl;
	cout << "\tGrades: ";	
	right.printGrades();
	cout << endl;
	return output;
}