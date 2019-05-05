// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.
#pragma once

#include "BSTNode.h"
#include "dictionary.h"
#define BNodeP BSTNode<Key, E>* 


using namespace std;

// Binary Search Tree implementation for the Dictionary ADT
template <typename Key, typename E>
class BST : public Dictionary<Key, E> {
private:
	BNodeP root;		// Root of the BST
	int nodecount;      // Number of nodes in the BST

	// Private "helper" functions
	void clearhelp(BNodeP);
	BNodeP inserthelp(BNodeP, BNodeP, BNodeP, const Key&, const E&); //takes root, parent, lost pointer, key, and value now
	BNodeP deletemin(BNodeP);
	BNodeP getmin(BNodeP);
	BNodeP removehelp(BNodeP, const Key&);
	E* findhelp(BNodeP, const Key&) const;
	void printhelp(BNodeP, int, char) const;	
	void vist(BNodeP) const;

public:
	BST() { root = NULL; nodecount = 0; }  // Constructor

	//Note from Prof Sipantzi -- I've commented out the destructor
	//since you would have to change clearhelp() to make it work with
	//doubly-threaded trees and that is not part of the assignment.
	//~BST() { clearhelp(root); }            // Destructor

	void clear(){clearhelp(root); root = NULL; nodecount = 0;}   // Reinitialize tree

	// Insert a record into the tree.
	// k Key value of the record.
	// e The record to insert.
	void insert(const Key& k, const E& e) {
		root = inserthelp(root, NULL, NULL, k, e);
		nodecount++;
	}

	// Remove a record from the tree.
	// k Key value of record to remove.
	// Return: The record removed, or NULL if there is none.
	E* remove(const Key& k) {
		E* temp = findhelp(root, k);   // First find it
		if (temp != NULL) {
			root = removehelp(root, k);
			nodecount--;
		}
		return temp;
	}
	// Remove and return the root node from the dictionary.
	// Return: The record removed, null if tree is empty.
	E* removeAny() {  // Delete min value
		if (root != NULL) {
			E* temp = new E;
			*temp = root->element();
			root = removehelp(root, root->key());
			nodecount--;
			return temp;
		}
		else return NULL;
	}

	// Return Record with key value k, NULL if none exist.
	// k: The key value to find. */
	// Return some record matching "k".
	// Return true if such exists, false otherwise. If
	// multiple records match "k", return an arbitrary one.
	E* find(const Key& k) const { return findhelp(root, k); }

	// Return the number of records in the dictionary.
	int size() { return nodecount; }

	void printInOrder() const {
		if (root == NULL) cout << "The BST is empty.\n";
		printhelp(root, 0, 'I');
		cout << endl;
	};
	void printReverse() const {
		if (root == NULL) cout << "The BST is empty.\n";
		else printhelp(root, 0, 'R');
		cout << endl;
	};

	void print() const {
		if (root == NULL) cout << "The BST is empty.\n";
		else printhelp(root, 0 , 'N');
		cout << endl;
	};
};

// Visit -- prints out root
template <typename Key, typename E>
void BST<Key, E>::vist(BNodeP r) const {
	cout << "Node - " << r->element() << endl;
}

// Clean up BST by releasing space back free store
template <typename Key, typename E>
void BST<Key, E>::clearhelp(BNodeP root) {
	if (root == NULL) return;
	clearhelp(root->left());
	clearhelp(root->right());
	delete root;
}

// Insert a node into the BST, returning the updated tree
template <typename Key, typename E>
BNodeP BST<Key, E>::inserthelp(BNodeP root, BNodeP parent, BNodeP temp, const Key& k, const E& it) {
	BNodeP newNode = new BSTNode<Key, E>(k, it, NULL, NULL);
	if (nodecount == 0) {//in the case of the first node being created return just a node
		return newNode;
	}

	if (root == NULL) { //once where needed to be inserted
		if (parent != NULL) { //if it has a parent
			if (newNode->key() < parent->key()) { //if it goes on the left
				newNode->setRight(parent);// new node gets right thread to parent
				newNode->setRThread(true);
				if (temp != NULL) { // If a pointer was lost during conversion
					newNode->setLeft(temp); //set it to the left so nothing is messed up
					newNode->setLThread(true);
				}
				return newNode;
			}
			else if (newNode->key() >= parent->key()) { //if node goes on the right
				newNode->setLeft(parent); //new node gets parent as thread to the left
				newNode->setLThread(true);
				if (temp != NULL) { //if pointer was lost
					newNode->setRight(temp); //set it to the right back to where parent had it
					newNode->setRThread(true);
				}
				return newNode;
			}
		}
	}
	if (root != NULL) {
		if (k < root->key()) { //if traversing the tree and going left
			temp = NULL;
			if (root->getLThread()) {//if left is a thread prepare to enter
				temp = root->left(); //save left pointer for later
				root->setLeft(NULL); //erase left pointer so no stack overflow
				root->setLThread(false); //not a thread
			}
			root->setLeft(inserthelp(root->left(), root, temp, k, it)); //enter next level
		}
		else if (k >= root->key()) { //if traversing the tree and going right
			temp = NULL;
			if (root->getRThread()) {//if right is a thread prepare to enter
				temp = root->right(); //save right pointer
				root->setRight(NULL); //erase right pointer so no double stack overflow
				root->setRThread(false); //change to not a thread
			}
			root->setRight(inserthelp(root->right(), root, temp, k, it)); //enter next level
		}
	}
	return root;
}

// Delete the minimum value from the BST, returning the revised BST
template <typename Key, typename E>
BNodeP BST<Key, E>::getmin(BNodeP rt) {
	if (rt->left() == NULL)
		return rt;
	else return getmin(rt->left());
}
template <typename Key, typename E>
BNodeP BST<Key, E>::deletemin(BNodeP rt) {
	if (rt->left() == NULL) // Found min
		return rt->right();
	else {                      // Continue left
		rt->setLeft(deletemin(rt->left()));
		return rt;
	}
}

// Remove a node with key value k
// Return: The tree with the node removed
template <typename Key, typename E>
BNodeP BST<Key, E>::removehelp(BNodeP rt, const Key& k) {
	if (rt == NULL) return NULL;    // k is not in tree
	else if (k < rt->key())
		rt->setLeft(removehelp(rt->left(), k));
	else if (k > rt->key())
		rt->setRight(removehelp(rt->right(), k));
	else {                            // Found: remove it
		BNodeP temp = rt;
		if (rt->left() == NULL) {     // Only a right child
			rt = rt->right();         //  so point to right
			delete temp;
		}
		else if (rt->right() == NULL) { // Only a left child
			rt = rt->left();          //  so point to left
			delete temp;
		}
		else {                    // Both children are non-empty
			BNodeP temp = getmin(rt->right());
			rt->setElement(temp->element());
			rt->setKey(temp->key());
			rt->setRight(deletemin(rt->right()));
			delete temp;
		}
	}
	return rt;
}

// Find a node with the given key value
template <typename Key, typename E>
E* BST<Key, E>::findhelp(BNodeP root, const Key& k) const {
	if (root == NULL) return NULL;          // Empty tree
	if (k < root->key())
		return findhelp(root->left(), k);   // Check left
	else if (k > root->key())
		return findhelp(root->right(), k);  // Check right
	else {
		E* temp = new E;
		*temp = root->element();
		return temp;  // Found it
	}
}

// Print out a BST
template <typename Key, typename E>
void BST<Key, E>::printhelp(BNodeP root, int level, char style) const { 
	switch (style) {
	case 'n':
	case 'N':{//JUST SAYING THIS PRINTS IT OUT MIRRORED
			if (root == NULL) {// Empty tree
				return;
			}
			if (!(root->getLThread())) {
				printhelp(root->left(), level + 1, 'N');   // Do left subtree
			}
			for (int i = 0; i < level; i++) {       // Indent to level
				cout << "  ";
			}
			cout << root->key() << "\n";        // Print node value
			if (!(root->getRThread())) {
				printhelp(root->right(), level + 1, 'N');  // Do right subtree
			}
			break;
		}
	case 'I':
	case 'i':
	{
			while (root->left() != NULL) {//go all the way left
				root = root->left();
			}
			while (root != NULL)
			{
				cout << root->key() << " ";//print out 

				if (root->getRThread()) {//if thread take thread
					root = root->right();
				}
				else if (root->right() != NULL) {//if regular take regular
					root = root->right();
					while (root->left() != NULL && !(root->getLThread())) {//go left all the way
						root = root->left();
					}
				}
				else {
					break;
				}
			}
			break;
		}
	case 'R':
	case 'r':
	{
			while (root->right() != NULL) {//go all the way right
				root = root->right();
			}
			while (root != NULL)
			{
				cout << root->key() << " ";//print out 

				if (root->getLThread()) {//if thread take thread
					root = root->left();
				}
				else if (root->left() != NULL) {//if regular take regular
					root = root->left();
					while (root->right() != NULL && !(root->getRThread())) {//go right all the way
						root = root->right();
					}
				}
				else {
					break;
				}
			}
		}
	}
}