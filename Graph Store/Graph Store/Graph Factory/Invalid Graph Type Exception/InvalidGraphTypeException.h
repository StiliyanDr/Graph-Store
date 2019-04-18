#ifndef __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__
#define __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__

#include "Runtime Error/RuntimeError.h"

class InvalidGraphTypeException : public RuntimeError
{
public:
	explicit InvalidGraphTypeException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit InvalidGraphTypeException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__