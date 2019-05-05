#pragma once
#include "dictionaryADT.h"
#include <string>
#include "myBag.h"
#include "kvpair.h"

#define KV KVpair<Key, Val>

template <typename Key, typename Val>
class  BDictionary : public Dictionary<Key, Val>
{
public:
	BDictionary() {};
	~BDictionary() {};

	void clear() {
		myBag.emptyBag();
	};

	bool insert(const Key& k, const Val& e) { //create KV and += to bag
		KV newKV(k, e);
		return (myBag += newKV);
	};

	bool remove(const Key& k, Val& rtnVal) {//create kv and remove, setting rtnval to newvalue
		KV newKV(k, rtnVal);
		if (myBag.remove(newKV)) {
			rtnVal = newKV.value();
			return true;
		}
		return false;
	};

	bool removeAny(Val& returnValue) {//inspect top to make sure something is there, remove top and update
		KV newKV;
		if (myBag.inspectTop(newKV)) {
			myBag.removeTop(newKV);
			returnValue = newKV.value();
			return true;
		}
	};

	bool find(const Key& k, Val& returnValue) const {// create kv, find, update
		KV newKV(k, returnValue);
		if (myBag.find(newKV)) {
			returnValue = newKV.value();
			return true;
		}
		else {	return false;	}		
	};

	int size() {
		return myBag.size();
	};
private:
	ABag<KV> myBag;
};