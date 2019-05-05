/* 
 * File:   SelfOrderedListADT.hpp
 * Author: Dad's ASUS
 *
 * Created on September 28, 2012, 8:18 AM
 */

#pragma once

template <typename E>
class SelfOrderedListADT {
public:
    //Default constructor/destructor
    SelfOrderedListADT(){}
    virtual ~SelfOrderedListADT() {}
    
    //Look for 'it'.  If found return true and execute the self-ordering
    //heuristic used for ordering the list: count, move-to-front, or transpose.
    //Increments the compare instance variable every time it compares 'it' to
    //other members of the list. Returns true if 'it' is found.
    virtual bool find(const E& it) = 0; 
    
    //Called by find if 'it' is not in the list. Adds the new 'it' to the list
    //Can also be called independently when initially loading the list with
    //unique values and when you want to load the list in the order 'it' is 
    //read without your re-order method being called (or the number of compares
    //being incremented.
    virtual void add(const E& it) = 0;  //Add new 'it' to the list

    
    virtual int getCompares() const = 0;   //Returns the number of accumulated compares
    virtual int size() const = 0;       //Returns the size of the list
    
    //Print the list
    //In order print of the list.  printlist() prints the entire list
    //whereas printlist(n) prints n nodes.
    virtual void printlist() const = 0;
    virtual void printlist(int n) const = 0;
    
};