#include "StudentInfo.h"
#include <string>
using namespace std;
//simplest class I have ever made. thank you for this
StudentInfo::StudentInfo(string newName, int eyedee, string major)
{
	setStudentName(newName);//strings are dynamically allocated and destroyed in implicit destructor
	setStudentID(eyedee);
	setMajor(major);
}
string StudentInfo::getStudentName(){//no comments needed for these functions. See title of function.
	return studentName;
}
int StudentInfo::getStudentID(){
	return studentID;
}
string StudentInfo::getMajor(){
	return Major;
}
void StudentInfo::setStudentName(string name){
	studentName = name;
}
void StudentInfo::setStudentID(int number){
	studentID = number;
}
void StudentInfo::setMajor(string major){
	Major = major;
}
