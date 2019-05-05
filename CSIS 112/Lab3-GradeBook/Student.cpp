#include "Student.h"
using namespace std;

Student::Student(string input){ //gets the name and up to ten grades for the student
	name = input;

	do{		
		cin >> inputGrade;

		setGrade(inputGrade);


	} while (inputGrade != -1 && gradeV.size() < 10);
}

void Student::setGrade(float grade){
	gradeV.push_back(grade); //push grade to student vector
}
void Student::setName(string last){
	name = last; //sets the name of student object
}
void Student::getGrades(){ //list the name and grades of student
	cout << name << endl; //name listing
	for (float i : gradeV) {
		cout << i << '\t'; //all grades in student vector listing
	}
	cout << endl;
}
string Student::getName(){
	return name; //gets the name of the student
}

