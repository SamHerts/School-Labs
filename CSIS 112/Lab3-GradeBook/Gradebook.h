#ifndef GRADEBOOK_H
#define GRADEBOOK_H
#include <string>
#include <iostream>
#include "Student.h"

class Gradebook
{	
public:
	Gradebook(std::string);
	void addStudent(std::string);
	void listStudents();
	int size();
	void listTitle();
	

private:
	std::string title;
	std::vector<Student> book;
	
	

};

#endif