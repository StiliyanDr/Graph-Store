#ifndef __GRAPH_CREATOR_HEADER_INCLUDED__
#define __GRAPH_CREATOR_HEADER_INCLUDED__

#include "String\String.h"
#include <memory>

class Graph;

class GraphCreator
{
public:
	GraphCreator(const GraphCreator&) = delete;
	GraphCreator& operator=(const GraphCreator&) = delete;
	virtual ~GraphCreator() = default;

	virtual std::unique_ptr<Graph> createGraph(const String& id) const = 0;
	const String& getCriterion() const;

protected:
	GraphCreator(const char* criterion);

private:
	void setCriterion(String criterion);

private:
	String criterion;
};

#endif //__GRAPH_CREATOR_HEADER_INCLUDED__