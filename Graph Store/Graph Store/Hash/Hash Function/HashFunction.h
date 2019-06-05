#ifndef __HASH_FUNCTION_HEADER_INCLUDED__
#define __HASH_FUNCTION_HEADER_INCLUDED__

template <class Key>
class HashFunction
{
public:
	unsigned operator()(const Key& key) const
	{
		return static_cast<unsigned>(key);
	}
};

/*
	The following class specialization implements the FNV hash algorithm:
	http://www.isthe.com/chongo/tech/comp/fnv/#top
*/

#include "String/String.h"

template <>
class HashFunction<String>
{
public:
	unsigned operator()(const String& key) const noexcept;

private:
	static const unsigned FNV_OFFSET_BASIS = 2166136261;
	static const unsigned FNV_PRIME = 16777619;
};

unsigned
HashFunction<String>::operator()(const String& key) const noexcept
{
	auto string = key.cString();
	auto hashValue = FNV_OFFSET_BASIS;

	while (*string)
	{
		hashValue *= FNV_PRIME;
		hashValue ^= *string;
		++string;
	}

	return hashValue;
}

#endif //__HASH_FUNCTION_HEADER_INCLUDED__