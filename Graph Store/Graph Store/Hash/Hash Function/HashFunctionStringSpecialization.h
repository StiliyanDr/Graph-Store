/*
	This class specialization implements the FNV hash algorithm:
	http://www.isthe.com/chongo/tech/comp/fnv/#top
*/

#ifndef __HASH_FUNCTION_STRING_SPECIALIZATION_HEADER_INCLUDED__
#define __HASH_FUNCTION_STRING_SPECIALIZATION_HEADER_INCLUDED__

#include "HashFunction.h"
#include "../../String/String.h"

template <>
class HashFunction<String>
{
	static const unsigned FNV_OFFSET_BASIS = 2166136261;
	static const unsigned FNV_PRIME = 16777619;

public:
	unsigned operator()(const String& key) const
	{
		const char* string = key.cString();

		unsigned hashValue = FNV_OFFSET_BASIS;
		
		while (*string)
		{
			hashValue *= FNV_PRIME;
			hashValue ^= *string;
			++string;
		}

		return hashValue;
	}
};

#endif //__HASH_FUNCTION_STRING_SPECIALIZATION_HEADER_INCLUDED__