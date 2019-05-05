#pragma once
#include "StudentInfo.h"
#include <vector>
#include <numeric>
#include <string>
#include <iostream>

class StudentGradeInfo : public StudentInfo
{
	friend std::ostream& operator<<(std::ostream&, StudentGradeInfo&);
public:
	StudentGradeInfo(std::string, int, std::string);	
	void setGrade(int);//adds a grade to vector
	void printGrades();//prints entire vector of grades NOTE: does not return anything
	double getAverage(); //returns average of grades
	bool operator==(int); //overloaded equivalence
private:
	std::vector<int> testGrades;
	double testAverage{ 0 };
};