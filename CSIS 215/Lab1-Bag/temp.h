#pragma once
#include "bagADT.h"
#include "kvpair.h"
#include <vector>
#include <algorithm>

typedef KVpair<int, string> KV;
typedef KVpair<string, int> KV;

class myBag : public Bag<KV>
{
public:
	myBag(int);
	~myBag();

	bool addItem(const KV item);		//
	bool remove(KV item);				//
	bool removeTop(KV returnValue);		//
	bool find(KV returnValue) const;	//
	bool inspectTop(KV item) const;		//
	void emptyBag();					//
	bool operator+=(const KV addend);	//
	int size() const;					//
	int bagCapacity() const;			//	

private:
	std::vector<KV> myVect;
	KV kv;
	int maxSize;
	int top = myVect.size();
};


myBag::myBag(int tempsize = 100) //Set arbitrary max size
{
	maxSize = tempsize;
}

myBag::~myBag()
{
}

bool myBag::addItem(const KV item) { //vector push back if less than arbitrary max size
	if (top < maxSize) {
		myVect.push_back(item);
		return true;
	}
	else {
		return false;
	}
};

bool myBag::remove(KV item) { //algo find then vector erase
	std::vector<KV>::iterator it;
	it = std::find(myVect.begin(), myVect.end(), item);
	if (it != (myVect.end())) {
		item = *it;
		myVect.erase(it);
		return true;
	}
	else {
		return false;
	}
};


bool myBag::removeTop(KV returnValue) {//vector back then vector pop back
	if (top > 0) {
		returnValue = myVect.back();
		myVect.pop_back();
		return true;
	}
	else {
		return false;
	}
};


bool myBag::find(KV returnValue) const { //run through and find else false
	for (KV obj : myVect) {
		if (obj == returnValue) {
			return (returnValue = obj);
		}
	}
	return false;
};


bool myBag::inspectTop(KV item) const { //vector back else false
	if (top > 0) {
		return (item = myVect.back());
	}
	else {
		return false;
	}
};


void myBag::emptyBag() { // vector clear
	myVect.clear();
};


bool myBag::operator+=(const KV addend) {// use bag addItem
	return this->addItem(addend);
};


int myBag::size() const { //vector size
	return top;
};


int myBag::bagCapacity() const { //get arbitrary size
	return maxSize;
};
