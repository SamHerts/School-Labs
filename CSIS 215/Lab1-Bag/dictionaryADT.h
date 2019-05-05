// From the software distribution accompanying the textbook
// "A Practical Introduction to Data Structures and Algorithm Analysis,
// Third Edition (C++)" by Clifford A. Shaffer.
// Source code Copyright (C) 2007-2011 by Clifford A. Shaffer.

#pragma once

// The Dictionary abstract class.
template <typename Key, typename E>
class  Dictionary {
public:
  Dictionary() {}          // Default constructor
  virtual ~Dictionary() {} // Base destructor

  // Reinitialize dictionary
  virtual void clear() = 0;

  // Insert a record
  // k: The key for the record being inserted.
  // e: The record being inserted.
  // Return true if insert is successful and false otherwise
  virtual bool insert(const Key& k, const E& e) = 0;

  // Looks for a record using the key and if found does the following:
  // - updates the E& rtnVal
  // - removes the record from the dictionary
  // - returns true
  // If the record is not found the function returns false.
  virtual bool remove(const Key& k, E& rtnVal) = 0;

  // Takes an arbitrary record from the dictionary and does the following:
  // - updates the E& returnValue
  // - removes the record from the dictionary
  // - returns true
  // If the dictionary is empty the function returns false.
  virtual bool removeAny(E& returnValue) = 0;

  // Looks for a record using the key and if found does the following:
  // - updates the E& returnValue
  // - returns true
  // If the record is not found the function returns false.
  virtual bool find(const Key& k, E& returnValue) const = 0;

  // Return the number of records in the dictionary.
  virtual int size() = 0;
};
