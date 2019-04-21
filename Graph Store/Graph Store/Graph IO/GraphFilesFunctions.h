#ifndef __GRAPH_TEXT_FILES_FUNCTIONS__
#define __GRAPH_TEXT_FILES_FUNCTIONS__

class String;
class Graph;

namespace GraphIO
{
	String getFileNameFor(const Graph& g);
	void removeFileFor(const Graph& g);
}

#endif //__GRAPH_TEXT_FILES_FUNCTIONS__