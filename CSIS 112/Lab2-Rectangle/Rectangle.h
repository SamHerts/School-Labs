#include <string>
#include <iostream>

class Rectangle{

	

public:
	Rectangle(){
		float width{ 0 };
		float length{ 0 };
	}
	void setLength(float input){
		length = input;
	}
	void setWidth(float input){
		width = input;
	}
	float getLength(){
		return length;
	}
	float getWidth(){
		return width;
	}
	void showData(){
		std::cout << "The length of the rectangle is " << getLength() << " units.\n";
		std::cout << "The width of the rectangle is " << getWidth() << " units.\n";
		std::cout << "The perimeter of the rectangle is " << calcPerimeter() << " units.\n";
		std::cout << "The area of the rectangle is " << calcArea() << " units.\n\n";
	}




private:

	float width;
	float length;

	float calcPerimeter(){
		return (2 * length + 2 * width);
	}

	float calcArea(){
		return (length*width);
	}


};
