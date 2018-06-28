#ifndef __GRAPH_CREATOR_HEADER_INCLUDED__
#define __GRAPH_CREATOR_HEADER_INCLUDED__

#include "String.h"
#include "Graph.h"
#include <memory>

class GraphCreator
{
public:
	GraphCreator(const GraphCreator&) = default;
	GraphCreator& operator=(const GraphCreator&) = delete;
	virtual ~GraphCreator() = default;

	virtual std::unique_ptr<Graph> createGraph(const char* identifier) const = 0;
	const String& getCriterion() const;

protected:
	GraphCreator(const char* criterion);

private:
	void setCriterion(String criterion);

private:
	String criterion;
};

#endif //__GRAPH_CREATOR_HEADER_INCLUDED__