#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "BufferPoolADT.h"
#include "constants.h"
#include "BufferBlock.h"
class LRUBufferPool : public BufferPoolADT
{
private:
	//The buffer pool consists of X number of buffer blocks
	vector<BufferBlock> myLRU;
	vector<int> list;
	std::string fileName;
public:
	//Constructor gets the filename of the file to be buffered,
	//opens the file, and instantiates poolSize buffer blocks by
	//reading the file and filling the blocks in order.  When the constructor
	//is done the buffer pool blocks should be full with the beginning
	//contents of the input file.
	LRUBufferPool() {}
	LRUBufferPool(std::string filename, int poolSize = 5, int blockSize = 4096)
	{
		fileName = filename;
		fstream input;
		int position = 0;
		input.open(fileName.c_str(), fstream::in | fstream::binary);
		if (input) {
			for (int i = 0; i < poolSize; i++) {//initialize pool to n items
				input.seekg(position);//move cursor in file to position
				char* myArr = new char[blockSize]; // create array to temporarily store data
				input.read(myArr, blockSize); // read data into temp array
				BufferBlock myBB(myArr, blockSize);// create bufferblock with data
				myBB.setID(i); // set ID
				myLRU.push_back(myBB);//add buffer to pool
				list.push_back(i); // add ID to list
				position += blockSize; // move cursor to new location
			}
		}
		else {
			cout << "Error opening file";
		}
		//input.close();
	}
	~LRUBufferPool() {}

	// Copy "sz" bytes from position "pos" of the buffered
	//   storage to "space".
	void getBytes(char* space, int sz, int pos) {

		vector<int>::iterator it;
		int blockNumber = pos / BLOCKSIZE;//Get the block number by c++ truncation
		int newPos = pos - BLOCKSIZE*blockNumber; //get the position inside the individual block
		for (BufferBlock i : myLRU) { //iterate over the pool
			if (blockNumber == i.getID()) { //if found
				i.getData(newPos, sz, space); //transfer data 
				it = find(list.begin(), list.end(), blockNumber);
				list.erase(it);//find the location in the list and remove it
				list.insert(list.begin(), blockNumber); //insert back into the front of the list
				return; //if its already in the list, leave function
			}
		} //if its not in the buffer list, it will get to here
		fstream input;
		int position = blockNumber*BLOCKSIZE; //position is the start of the block
		input.open(fileName.c_str(), fstream::in | fstream::binary); //open file
		if (input) {//ensure open
			input.seekg(position);//move to beginning of the block
			char* myArr = new char[BLOCKSIZE];			// create storage array
			input.read(myArr, BLOCKSIZE);				// read the block into array
			BufferBlock newBlock(myArr, BLOCKSIZE);		//create new block with data from array
			newBlock.getData(newPos, sz, space);		//Get data to pass back to main
			newBlock.setID(blockNumber);				// set ID based on block number
			myLRU.erase(myLRU.begin() + list.back());	// erase the buffer block from list
			list.pop_back();							// remove least recently used
			myLRU.insert(myLRU.begin(), newBlock);		// insert most recently used into front
			list.insert(list.begin(), blockNumber);		//insert most recently used into front
		}
		else {
			cout << "Error opening File" << endl;
		}
		input.close();
	}

	// Print the order of the buffer blocks using the block id
	//	 numbers.
	void printBufferBlockOrder() {//range based for loop
		for (int i : list) {
			cout << i << endl;
		}
	}

	// Get the block id number of the least recently used 
	//	 buffer block.
	int getLRUBlockID() { //least recently used will be the rear-most item on the list. Always
		return list.back();
	}
};