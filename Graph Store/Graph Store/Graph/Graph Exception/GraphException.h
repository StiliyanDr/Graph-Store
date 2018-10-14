#ifndef __GRAPH_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_EXCEPTION_HEADER_INCLUDED__

#include "../../Runtime Error/RuntimeError.h"

class GraphException : public RuntimeError
{
public:
	explicit GraphException(String errorMessage) :
		RuntimeError(std::move(errorMessage))
	{
	}

	explicit GraphException(const std::string& errorMessage) :
		RuntimeError(errorMessage)
	{
	}
};

#endif //__GRAPH_EXCEPTION_HEADER_INCLUDED__