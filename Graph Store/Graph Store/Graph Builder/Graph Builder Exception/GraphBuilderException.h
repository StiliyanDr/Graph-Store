#ifndef __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class GraphBuilderException : public std::runtime_error
{
public:
	explicit GraphBuilderException(const char* errorMessage) :
		runtime_error(errorMessage)
	{
	}

	explicit GraphBuilderException(const std::string& errorMessage) :
		runtime_error(errorMessage)
	{
	}
};

#endif //__GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__