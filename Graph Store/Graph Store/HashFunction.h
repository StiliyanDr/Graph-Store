/*
	The hash function used in the Hash data structure.
	This class implements an identity function: the argument 
	is casted to size_t and the result is returned.
	Template class specialization should be used for different behaviour.
*/

#ifndef __HASH_FUNCTION_HEADER_INCLUDED__
#define __HASH_FUNCTION_HEADER_INCLUDED__

template <class Key>
class HashFunction
{
public:
	size_t operator()(const Key& key) const
	{
		return static_cast<size_t>(key);
	}
};

#endif //__HASH_FUNCTION_HEADER_INCLUDED__