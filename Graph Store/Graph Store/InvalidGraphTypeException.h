#ifndef __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__
#define __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__

#include "GraphException.h"

class InvalidGraphTypeException : public GraphException
{
public:
	explicit InvalidGraphTypeException(const char* errorMessage);
	explicit InvalidGraphTypeException(const std::string& errorMessage);
	virtual ~InvalidGraphTypeException() = default;
};

#endif //__INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__