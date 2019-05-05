#pragma once

#include <iostream>
#include "Tile.h"

class Ship : public Tile
{
public:
	Ship(int, int);
	~Ship();
	bool isSunk();
	void HIT();
	bool isShip() { return amShip; };
	int shipsLeft();

private:	
	bool Sunk;	
	static int shipTotal;
	int hitCount = 0;
	bool amShip = true;
};

int Ship::shipTotal = 0;

Ship::Ship(int xcoord, int ycoord) : Tile(xcoord,ycoord)
{	
	Sunk = false;
	shipTotal++;
}
Ship::~Ship()
{
	shipTotal--;
}
bool Ship::isSunk(){
	return Sunk;
}
void Ship::HIT(){
	if (!isSunk()){
		Sunk = true;
		hitCount++;
		std::cout << "HIT!\n";
	}
	else{
		hitCount++;
		std::cout << "HIT AGAIN!\n";
	}
}
int Ship::shipsLeft(){
	return shipTotal;
}




