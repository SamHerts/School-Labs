/* 
 * File:   constants.hpp
 * Author: Prof Terri Sipantzi
 *
 * Created on August 25, 2012, 10:09 AM
 */

#pragma once

#include <iostream>
using namespace std;

static const int BLOCKSIZE = 4096;	//buffer blocksize
static const int POOL_SIZE = 5;   	//number of buffer block in the buffer pool

//common char functions
//create and initialize a char array and then return a pointer to it
void initializeCharArray(int sz, char* ch) {
    for (int i=0; i<sz; i++) {
        ch[i] = (char) NULL;
    }
}

//get a new char array and initialize it
char* getCharArray(int sz) {
    char* myChars = new char[sz];
    initializeCharArray(sz, myChars);
    return myChars;
}

//print out a string of chars
void printChars(char* ch, int sz, int blkid) {
    cout << "My data for block " << blkid << " is: \"";
    for (int i=0; i<sz; i++) {
        cout << ch[i];
    }
    cout << "\"\n";
}