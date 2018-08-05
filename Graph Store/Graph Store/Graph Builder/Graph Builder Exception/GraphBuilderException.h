#ifndef __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class GraphBuilderException : public std::runtime_error
{
public:
	explicit GraphBuilderException(const char* errorMessage);
	explicit GraphBuilderException(const std::string& errorMessage);
	virtual ~GraphBuilderException() = default;
};

#endif //__GRAPH_BUILDER_EXCEPTION_HEADER_INCLUDED__