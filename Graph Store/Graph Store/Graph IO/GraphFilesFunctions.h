#ifndef __GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__
#define __GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__

class String;
class Graph;
class GraphCollection;

namespace GraphIO
{
	GraphCollection loadDirectory(const String& path);
	void removeFileFor(const Graph& g);
	String getFileNameFor(const Graph& g);
}

#endif //__GRAPH_FILES_FUNCTIONS_HEADER_INCLUDED__