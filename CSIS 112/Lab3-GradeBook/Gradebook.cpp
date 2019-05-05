#include "Gradebook.h"
using namespace std;

Gradebook::Gradebook(string input){
	title = input;
}
void Gradebook::addStudent(string newname){	
	
		Student newStudent(newname);	//Create new Student object	
		book.push_back(newStudent); //add new student to gradebook vector
	}


void Gradebook::listStudents(){
	for (Student i : book){
		i.getGrades();
		cout << endl;
	}	
}
int Gradebook::size(){
	return book.size(); //gets the size of the gradebook vector
}
void Gradebook::listTitle(){
	cout << "\t\t\t You are viewing the gradebook " << title << endl;
}
