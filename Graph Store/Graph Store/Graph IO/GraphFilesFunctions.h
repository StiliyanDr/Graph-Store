#ifndef __GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__
#define __GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__

class String;
class Graph;

namespace GraphIO
{
	String getFileNameFor(const Graph& g);
	void removeFileFor(const Graph& g);
}

#endif //__GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__