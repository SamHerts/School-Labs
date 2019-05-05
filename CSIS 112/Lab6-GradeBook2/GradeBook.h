#pragma once
#include "StudentGradeInfo.h"
#include <string>
class GradeBook
{
	friend bool sortVector(StudentGradeInfo&, StudentGradeInfo&);	
public:
	GradeBook(std::string); //creates gradebook with name	
	std::string getTitle(); //Returns title of Gradebook
	void addStudent(); //adds a new student
	void listStudents();//lists all students
	int size();//returns size of gradebook
private:
	std::string Title;
	std::vector<StudentGradeInfo> students;
};
