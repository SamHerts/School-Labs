#pragma once

#include <iostream>
class Tile
{
public:
	Tile(int, int);
	~Tile();
	virtual void HIT() = 0;
	friend std::ostream& operator<<(std::ostream&, const Tile*);
	virtual bool isShip() = 0;

	

private:
	int x;
	int y;

};

Tile::Tile(int xcoord, int ycoord)
{
	x = xcoord;
	y = ycoord;
}

Tile::~Tile()
{
}

std::ostream& operator<<(std::ostream& os,  Tile* s)
{
	
	if ((*s).isShip()){
		os << '#';
	}
	else{
		os << '~';
	}
	
	return os;
}
