
#pragma once
#include "SelfOrderedListADT.h"
#include "llist.h"


template <typename E>
class SOL : public SelfOrderedListADT<E>
{
public:
	SOL(int type) { orderType = type; };
	~SOL() {};
	//Look for 'it'.  If found return true and execute the self-ordering
	//heuristic used for ordering the list: count, move-to-front, or transpose.
	//Increments the compare instance variable every time it compares 'it' to
	//other members of the list. Returns true if 'it' is found.
	bool find(const E& it) {
		 
		myList.moveToStart(); //start at the beginning of the list

		for (int i = 0; i < size(); i++) {	//loop over whole list		
			compareCount++;//every compare, increment counter
			if (it == myList.getValue()) {//if E it is equivalent to curr's value
				
				myList.setFrequency(myList.getFrequency() + 1); //increment curr's frequency
				reOrder(); //reorder using whatever heuristic
				return true; // leave for loop
			}
			myList.next(); // move to next to keep checking
		}		
		add(it); //if still not found in entire list, add it to the end		
		return false;
	};

	//Called by find if 'it' is not in the list. Adds the new 'it' to the list
	//Can also be called independently when initially loading the list with
	//unique values and when you want to load the list in the order 'it' is 
	//read without your re-order method being called (or the number of compares
	//being incremented.
	void add(const E& it) {
		myList.append(it);
	};  //Add new 'it' to the list

	int getCompares() const { return compareCount; };   //Returns the number of accumulated compares
	int size() const { return myList.length(); };       //Returns the size of the list

	//In order print of the list.  printlist() prints the entire list
	//whereas printlist(n) prints n nodes.
	void printlist() const { myList.print(); };
	void printlist(int n) const { myList.print(n); };
	void reOrder() {
		if (myList.isHead()) { return; }
		if (orderType == 0) {//Count Order			
			
			int freq = myList.getFrequency(); //save the frequency			
			E temp = myList.remove(); //save the item and remove it from list			
			myList.moveToStart(); // start at the beginning			
			for (int i = 0; i < size(); i++) { //for every item in the list
				if (myList.getFrequency() >= freq) { // if the current node has higher frequency, move on to the next					
					myList.next();					
				}
				else {//if desired frequency is greater than max frequency
					
					myList.insert(temp); //insert at new position
					myList.setFrequency(freq); //reset frequency
					return;
				}
			}				
		}
		else if (orderType == 1) {// Move to front Order		
			
			int freq = myList.getFrequency(); //save the current frequency
			E temp = myList.remove(); //remove the current and save its value
			myList.moveToStart(); //move to beginning
			myList.insert(temp);//insert at beginning
			myList.setFrequency(freq); //set curr's frequency
			return;
		}
		else if (orderType == 2) {//Translocate Order			

			int place = myList.currPos(); // save the current position
			int freq = myList.getFrequency(); //save the current frequency
			E temp = myList.remove(); // save the current and remove it from list
			myList.moveToPos(place - 1); //move backwards by one position
			myList.insert(temp); //insert at position -1
			myList.setFrequency(freq);// set curr's frequency
			return;
		}
	}

private:
	int compareCount = 0;
	LList<E> myList;
	int orderType;
};