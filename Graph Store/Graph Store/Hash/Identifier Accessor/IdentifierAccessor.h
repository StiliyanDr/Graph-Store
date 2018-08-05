#ifndef __IDENTIFIER_ACCESSOR_HEADER_INCLUDED__
#define __IDENTIFIER_ACCESSOR_HEADER_INCLUDED__

#include "../../String/String.h"

class IdentifierAccessor
{
public:
	template <class T>
	const String& operator()(const T& object) const
	{
		return object.getIdentifier();
	}
};

#endif //__IDENTIFIER_ACCESSOR_HEADER_INCLUDED__