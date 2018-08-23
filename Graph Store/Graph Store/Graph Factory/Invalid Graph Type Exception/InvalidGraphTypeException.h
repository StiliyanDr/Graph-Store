#ifndef __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__
#define __INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class InvalidGraphTypeException : public std::runtime_error
{
public:
	explicit InvalidGraphTypeException(const char* errorMessage) :
		runtime_error(errorMessage)
	{
	}

	explicit InvalidGraphTypeException(const std::string& errorMessage) :
		runtime_error(errorMessage)
	{
	}
};

#endif //__INVALID_GRAPH_TYPE_EXCEPTION_HEADER_INCLUDED__