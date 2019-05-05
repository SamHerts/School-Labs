#pragma once

#include "book.h"
#include "link.h"
#include "list.h"


// This is the declaration for LList. It is split into two parts
// because it is too big to fit on one book page
// Linked list implementation
template <typename E> class LList: public List<E> {
private:
  Link<E>* head;       // Pointer to list header
  Link<E>* tail;       // Pointer to last element
  Link<E>* curr;       // Access to current element
  int cnt;    	       // Size of list

  void init() {        // Intialization helper method
    curr = tail = head = new Link<E>;
    cnt = 0;
  }

  void removeall() {   // Return link nodes to free store
    while(head != NULL) {
      curr = head;
      head = head->next;
      delete curr;
    }
  }

public:
  LList() { init(); }    // Constructor
  ~LList() { removeall(); }                   // Destructor
  void print() const {// Print list contents
	  if (head == NULL) {
		  cout << "Empty List" << endl;
		  return;
	  }
	  Link<E>* temp = head->next;
	  while (temp != NULL) {
		  cout << temp->element << '-' << temp->freq << " ";
		  temp = temp->next;
	  }
  };                
  void print(int n) const { //Print list contents up to Nth
	  if (head == NULL) {
		  cout << "Empty List" << endl;
		  return;
	  }
	  Link<E>* temp = head->next;
	  for (int i = 0; i < n; i++) {
		  if (temp != NULL) {
			  cout << temp->element << '-' << temp->freq << " ";
			  temp = temp->next;
		  }
	  }
  };

  void clear() { removeall(); init(); }       // Clear list

  // Insert "it" at current position
  void insert(const E& it) {
    curr->next = new Link<E>(it, curr->next);  
	if (tail == curr) {
		tail = curr->next;  // New tail
	}
    cnt++;
  }

  void append(const E& it) { // Append "it" to list
    tail = tail->next = new Link<E>(it, NULL);
    cnt++;
  }

  // Remove and return current element
  E remove() {
    Assert(curr->next != NULL, "No element");
    E it = curr->next->element;      // Remember value
    Link<E>* ltemp = curr->next;     // Remember link node
    if (tail == curr->next) tail = curr; // Reset tail
    curr->next = curr->next->next;   // Remove from list
    delete ltemp;                    // Reclaim space
    cnt--;			     // Decrement the count
    return it;
  }

  void setFrequency( int i) {	curr->next->freq = i;	}
  int getFrequency() {	return curr->next->freq;	}

  void moveToStart() // Place curr at list start
  {	curr = head;	}

  void moveToEnd()   // Place curr at list end
  {	curr = tail;	}

  // Move curr one step left; no change if already at front
	void prev() {
		 if (curr == head) return;       // No previous element
		 Link<E>* temp = head;
		 // March down list until we find the previous element
		 while (temp->next != curr) temp = temp->next;
		 curr = temp;
	 }

  // Move curr one step right; no change if already at end
  void next()
    { if (curr != tail) curr = curr->next; }

  int length() const  { return cnt; } // Return length

  // Return the position of the current element
  int currPos() const {
    Link<E>* temp = head;
    int i;
    for (i=0; curr != temp; i++)
      temp = temp->next;
    return i;
  }

  // Move down list to "pos" position
  void moveToPos(int pos) {
    Assert ((pos>=0)&&(pos<=cnt), "Position out of range");
    curr = head;
    for(int i=0; i<pos; i++) curr = curr->next;
  }

  const E& getValue() const { // Return current element
	  Assert(curr->next != NULL, "No value");
	  return curr->next->element;
  } 

  bool isHead() {	return (curr == head);  }
 };

