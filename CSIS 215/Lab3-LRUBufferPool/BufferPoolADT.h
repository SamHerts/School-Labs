/* 
 * File:   BufferPoolADT.h
 * Author: Clifford A. Shaffer, Dept of CompSci, Virginia Tech
 *         Page 286 of Data Structures and Algorithm Analysis
 *         Edition 3.2 (C++ Version)
 * Modified by Prof Terri Sipantzi, Dept of Computational Sciences,
 *		   Liberty University, 3/28/2014
 *
 * Created on June 5, 2012
 */

#pragma once

#include <string>

// ADT for buffer pools using the message-passing style
class BufferPoolADT {
private:
	//The buffer pool consists of X number of buffer blocks
	
public:
    //Constructor gets the filename of the file to be buffered,
	//opens the file, and instantiates poolSize buffer blocks by
	//reading the file and filling the blocks in order.  When the constructor
	//is done the buffer pool blocks should be full with the beginning
	//contents of the input file.
	BufferPoolADT() {}
    BufferPoolADT(std::string filename, int poolSize = 5, int blockSize = 4096) {}
    virtual ~BufferPoolADT() {}
    
    // Copy "sz" bytes from position "pos" of the buffered
    //   storage to "space".
    virtual void getBytes(char* space, int sz, int pos) = 0;
	
	// Print the order of the buffer blocks using the block id
	//	 numbers.
	virtual void printBufferBlockOrder() = 0;
	
	// Get the block id number of the least recently used 
	//	 buffer block.
	virtual int getLRUBlockID() = 0;
};