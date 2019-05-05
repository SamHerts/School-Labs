#pragma once
#include <iostream>

class Tile
{
public:
	Tile(int, int, bool);// take in x coord, y coord, and ship||water
	void HIT();

	int shipsLeft() { return shipTotal; };
	bool getShip() { return amShip; };
	char getChar() { return self; };
	bool getSunk() { return Sunk; };
	void setChar(char temp) { self = temp; };
	void setSunk(bool temp) { Sunk = temp; };
	void setShip(bool temp) { amShip = temp; };

	friend std::ostream& operator<<(std::ostream&, const Tile*); //overloaded << operator to allow printing ship type
	void operator=(Tile*); //overloaded = operator to allow setting objects equal to each other

private:
	char self; //stores ship icon
	bool Sunk; // stores if sunk or not
	static int shipTotal; // stores total number of ships
	int hitCount = 0; //stores how many times ship has been hit
	bool amShip; //stores if ship or water -- true=ship, false=water
	int x; //stores x coord
	int y; // stores y coord
};

int Tile::shipTotal = 0;//set ship total to 0 to start.

Tile::Tile(int xcoord, int ycoord, bool tempShip)
{
	setShip(tempShip);//set if object is ship or water
	setSunk(false);//start off floating
	if (getShip()) {//if object is ship, add to total and set icon
		shipTotal++;
		setChar('#');
	}
	else {//else just set icon
		setChar('~');
	}
}

void Tile::HIT() {//test whether user input is ship or water
	if (getShip()) {//if is a ship 
		if (!getSunk()) { // and is not sunk, sink, decrease counter, and update icon.
			setSunk(true);
			hitCount++;
			std::cout << "HIT!\n";
			shipTotal--;
			setChar('X');
		}
		else {//if is ship and already sunk, update icon
			hitCount++;
			std::cout << "HIT AGAIN!\n";
			if (hitCount % 2 == 0) {
				setChar('+');
			}
			else {
				setChar('X');
			}
		}
	}
	else {//if not ship, miss and set icon
		std::cout << "MISS!\n";
		setChar('O');
	}
}

std::ostream& operator<<(std::ostream& os, Tile* s)//output icon of object
{
	os << (*s).getChar();
	return os;
}

void Tile::operator=(Tile* Right) {//set all values of new object to old object
	setChar(Right->getChar());
	setSunk(Right->getSunk());
	setShip(Right->getShip());
	x = Right->x;
	y = Right->y;
}
