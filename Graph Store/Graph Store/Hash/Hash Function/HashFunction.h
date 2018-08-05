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

#endif //__HASH_FUNCTION_HEADER_INCLUDED__