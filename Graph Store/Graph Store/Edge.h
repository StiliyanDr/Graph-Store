#ifndef __EDGE_HEADER_INCLUDED__
#define __EDGE_HEADER_INCLUDED__

class Vertex;

class Edge
{
	friend class GraphBase;

public:
	~Edge() = default;

	Vertex& getVertex();
	unsigned getWeight() const;

private:
	Edge(Vertex* vertex, unsigned weight = 1);
	Edge(const Edge&) = default;
	Edge& operator=(const Edge&) = default;

	void setVertex(Vertex* vertex);
	void setWeight(unsigned weight);
	
private:
	unsigned weight;
	Vertex* vertex;
};

#endif //__EDGE_HEADER_INCLUDED__