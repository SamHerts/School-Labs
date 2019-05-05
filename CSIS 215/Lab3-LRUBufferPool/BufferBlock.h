#pragma once

#include <iostream>
#include <string>
#include "BufferBlockADT.h"
#include "constants.h"

using namespace std;

class BufferBlock : public BufferblockADT
{
public:
	BufferBlock() {}
	BufferBlock(char* data, int sz = 4096) {
		setBlock(data);
	}
	~BufferBlock() {}

	//read the block from pos to pos + sz-1 (or to the end of the block)
	void getData(int pos, int sz, char* data) {
		strncpy(data, block + pos, pos+(sz-1));
	}

	//setID
	void setID(int id) {
		blockID = id;
	}

	//getID
	int getID() const {	
		return blockID;
	}

	//getBlocksize
	int getBlocksize() const {
		return BLOCKSIZE;
	}

	//return the block
	char* getBlock() const {
		return block;
	}

	//set the block
	void setBlock(char* blk) {
		block = blk;
	}

private:
	int blockID;
	char* block;

};