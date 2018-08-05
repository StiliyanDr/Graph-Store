#ifndef __GRAPH_EXCEPTION_HEADER_INCLUDED__
#define __GRAPH_EXCEPTION_HEADER_INCLUDED__

#include <stdexcept>

class GraphException : public std::runtime_error
{
public:
	explicit GraphException(const char* errorMessage);
	explicit GraphException(const std::string& errorMessage);
	virtual ~GraphException() = default;
};

#endif //__GRAPH_EXCEPTION_HEADER_INCLUDED__