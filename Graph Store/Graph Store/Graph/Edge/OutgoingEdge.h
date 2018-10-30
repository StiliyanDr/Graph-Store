#ifndef __OUTGOING_EDGE_HEADER_INCLUDED__
#define __OUTGOING_EDGE_HEADER_INCLUDED__

class Vertex;

class OutgoingEdge
{
	friend class GraphBase;

public:
	const Vertex& getVertex() const;
	Vertex& getVertex();
	unsigned getWeight() const;

private:
	OutgoingEdge(Vertex* v, unsigned weight = 1);

	void setVertex(Vertex* v);
	void setWeight(unsigned weight);

private:
	unsigned weight;
	Vertex* vertex;
};

#endif //__OUTGOING_EDGE_HEADER_INCLUDED__