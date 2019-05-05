// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

#pragma once

// Container for a key-value pair
// Key object must be an object for which the == operator is defined.
// For example, int and string will work since they both have == defined,
// but Int will not work since it does not have == defined.
template <typename Key, typename E>
class KVpair {
private:
	Key k;
	E e;
public:
	// Constructors
	KVpair() {}
	KVpair(Key kval, E eval)
	{
		k = kval; e = eval;
	}
	KVpair(const KVpair& o)  // Copy constructor
	{
		k = o.k; e = o.e;
	}

	void operator =(const KVpair& o) // Assignment operator
	{
		k = o.k;
		e = o.e;		
	}

	bool operator==(const KVpair& o) const {
		if (o.k == k) {
			return true;
		}
		return false;
	}

	// Data member access functions
	Key key() { return k; }
	void setKey(Key ink) { k = ink; }
	E value() { return e; }
};