#pragma once
#include "bagADT.h"
#include "kvpair.h"
#include <vector>
#include <algorithm>

template <typename E>
class ABag : public Bag<E>
{
public:
	ABag(int tempsize = 100) //Set arbitrary max size
	{
		maxSize = tempsize;
	};
	~ABag(){};

	bool addItem(const E& item) { //vector push back if less than arbitrary max size
		if (myVect.size() < this->bagCapacity()) {
			myVect.push_back(item);
			return true;
		}
		else {
			return false;
		}
	};

	bool remove(E& item) { //algo find then vector erase
		std::vector<E>::iterator it; //create iterator object
		it = std::find(myVect.begin(), myVect.end(), item); //use algorithm find because it returns vector iterator
		if (it != (myVect.end())) {//if not passed the end of the vector
			item = *it; //set value of iterator to searched item
			myVect.erase(it); //remove from vector
			return true;
		}
		else {
			return false;
		}
	};
	bool removeTop(E& returnValue) {//vector back then vector pop back
		if (myVect.size() > 0) {
			returnValue = myVect.back();//gives the end item in the vector
			myVect.pop_back();//removes the end item in the vector
			return true;
		}
		else {
			return false;
		}
	};

	bool find(E& returnValue) const { //run through and find else false
		for (E obj : myVect) {
			if (obj == returnValue) {//overloaded equivalence operator from KVpairs
				returnValue = obj;
				return true;
			}
		}
		return false;
	};

	bool inspectTop(E& item) const { //vector back else false
		if (myVect.size() > 0) {
			item = myVect.back();
			return true;
		}
		else {
			return false;
		}
	};

	void emptyBag() { // vector clear
		myVect.clear();
	};

	bool operator+=(const E& addend) {// use bag addItem
		return this->addItem(addend);
	};

	int size() const { //vector size
		return myVect.size();
	};

	int bagCapacity() const { //get arbitrary size
		return maxSize;
	};

private:
	std::vector<E> myVect;
	int maxSize;
};
