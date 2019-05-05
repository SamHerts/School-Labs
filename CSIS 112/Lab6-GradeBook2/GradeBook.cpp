#include "GradeBook.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

GradeBook::GradeBook(string input)
{
	Title = input;
}
string GradeBook::getTitle(){
	return Title;
}

void GradeBook::addStudent(){
	string name1;
	int eyedee;
	string major;
	bool flag{ true };

	if (size() < 50){//ensure gradebook is at most 50
		cout << "Input student name: " << endl; //get name
		getline(cin, name1);
		

		cout << "Input student Major: " << endl; //get major
		getline(cin, major);
		

		cout << "Enter new unique Student ID: " << endl; //get ID
		cin >> eyedee;
		for (StudentGradeInfo i : students){ //ensure ID is unique
			if (i == eyedee){
				cout << "ID already in use, please try again: " << endl;
				cin >> eyedee;
			}
		}
		StudentGradeInfo newStudent(name1, eyedee, major);//create new student
		students.push_back(newStudent); //puts new student onto vector

	}
	else{
		cout << "Gradebook is full, please create a new Gradebook to add more than 50 students." << endl << endl;//lets user know that 50 students have been added
	}

}
void GradeBook::listStudents(){

	sort(students.begin(), students.end(), sortVector);//sorting list based on ID functor. Gotten from cplusplus resource regarding sort() function
	for (StudentGradeInfo i : students){
		cout << i;
	}
}
int GradeBook::size(){
	return students.size();//gets size of student vector
}
bool sortVector(StudentGradeInfo& left, StudentGradeInfo& right)//functor for sorting by ID
{
	return left.getStudentID() < right.getStudentID(); //using normal less-than operator
}