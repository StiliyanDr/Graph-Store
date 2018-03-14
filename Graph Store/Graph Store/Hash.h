/*
	Hash data structure that stores addresses of objects
	of type Item, whose key is of type Key. KeyAccessor
	is used to access an item's key.

	The method used for collision resolution is linear probing.
	
	Type requirements:

	(1) Key must have overloaded operator!= .
	(2) HashFunction<Key> must be default constructable.
	(3) HashFunction<Key>'s operator() must take an object of type Key by
	    const& and return its hash value (unsigned integer).
	(4) KeyAccessor must be default constructable. 
	(5) KeyAccessor's operator() must take an object of type Item
		by const& and return its key of type Key by const&.
*/

#ifndef __HASH_HEADER_INCLUDED__
#define __HASH_HEADER_INCLUDED__

#include "DArray.h"
#include "HashFunction.h"

template <class Item, class Key, class KeyAccessor>
class Hash
{
public:
	explicit Hash(int expectedCount);
	Hash(Hash<Item, Key, KeyAccessor>&& source);
	Hash(const Hash<Item, Key, KeyAccessor>& source) = default;
	Hash<Item, Key, KeyAccessor>& operator=(Hash<Item, Key, KeyAccessor>&& rhs);
	Hash<Item, Key, KeyAccessor>& operator=(const Hash<Item, Key, KeyAccessor>& rhs);
	~Hash() = default;

	Item* search(const Key& key);
	void insert(Item& item);
	Item* remove(const Key& key);

	size_t getCount() const;
	bool isEmpty() const;
	void empty();

private:
	//The minimum number of slots in the table.
	static const int MIN_SIZE = 3;

private:
	size_t count;			   //The number of elements in the table.
	DArray<Item*> table;	   //The table of pointers to store Item addresses. 
	KeyAccessor accessor;      //A functor that gives access to an item's key.
	HashFunction<Key> hash;    //The hash function.

private:
	//Not const since the hash functor may have non const operator()!
	int getIndex(const Key& key); 
	void resize(int newSize);
	void nullify(DArray<Item*>& arr);
	void rehash(int index);
	void swapContentsWith(Hash<Item, Key, KeyAccessor> temp);
	void stealTableFrom(Hash<Item, Key, KeyAccessor>& other);
};

#include "Hash.hpp"

#endif //__HASH_HEADER_INCLUDED__