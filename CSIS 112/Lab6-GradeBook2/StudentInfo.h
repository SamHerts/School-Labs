#pragma once
#include <string>

class StudentInfo
{
public:
	StudentInfo(std::string, int, std::string);	
	std::string getStudentName();//returns students name
	int getStudentID(); //returns student ID
	std::string getMajor(); //returns student major
	void setStudentName(std::string); 
	void setStudentID(int);
	void setMajor(std::string);
private:
	std::string studentName;
	int studentID;
	std::string Major;
};

