#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <vector>
#include <string>




class Student
{	

public:
	Student::Student(std::string); 
	void setGrade(float); 
	void setName(std::string);
	void getGrades();
	std::string getName();

private:
	float inputGrade{ 0 };
	std::vector<float> gradeV;
	std::string name;
};

#endif
