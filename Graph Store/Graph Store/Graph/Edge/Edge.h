#ifndef __EDGE_HEADER_INCLUDED__
#define __EDGE_HEADER_INCLUDED__

class Vertex;

class Edge
{
	friend class GraphBase;

public:
	const Vertex& getVertex() const;
	unsigned getWeight() const;

private:
	Edge(Vertex* v, unsigned weight = 1);

	void setVertex(Vertex* v);
	void setWeight(unsigned weight);

private:
	unsigned weight;
	Vertex* vertex;
};

#endif //__EDGE_HEADER_INCLUDED__