#pragma once

#include "Tile.h"
#include <iostream>


class Water : public Tile

{
public:
	Water(int, int);
	~Water();	
	void HIT();
	bool isShip() {	return amShip;	};

private:
	bool amShip = false;

};

Water::Water(int xcoord, int ycoord) : Tile(xcoord,ycoord)
{

}

Water::~Water()
{
}

void Water::HIT(){
	std::cout << "Miss!\n"; 
}
