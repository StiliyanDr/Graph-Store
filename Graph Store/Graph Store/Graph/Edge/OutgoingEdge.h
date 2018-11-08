#ifndef __OUTGOING_EDGE_HEADER_INCLUDED__
#define __OUTGOING_EDGE_HEADER_INCLUDED__

class Vertex;

class OutgoingEdge
{
	friend class Graph;

public:
	typedef unsigned Weight;

public:
	const Vertex& getVertex() const;
	Vertex& getVertex();
	Weight getWeight() const;

private:
	OutgoingEdge(Vertex* v, Weight w = 1);

	void setVertex(Vertex* v);
	void setWeight(Weight w);

private:
	Weight weight;
	Vertex* vertex;
};

#endif //__OUTGOING_EDGE_HEADER_INCLUDED__