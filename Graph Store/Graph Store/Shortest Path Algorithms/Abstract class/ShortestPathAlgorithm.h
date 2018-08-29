#ifndef __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "../../String/String.h"
#include "../../Distance/Distance.h"
#include <forward_list>

class Graph;
class Vertex;

class ShortestPathAlgorithm
{
	struct DecoratedVertex
	{
		DecoratedVertex(const Vertex& originalVertex) :
			originalVertex(originalVertex),
			parent(nullptr),
			distanceToSource(Distance::getInfinity())
		{
		}

		const Vertex& originalVertex;
		const DecoratedVertex* parent;
		Distance distanceToSource;
	};

	class Path
	{
	public:
		Path(const Path&) = default;
		Path& operator=(const Path&) = default;
		Path(Path&& source);
		Path& operator=(Path&& rhs);
		~Path() = default;

		void print(std::ostream& out) const;

	private:
		static bool existsPathBetween(const DecoratedVertex& source, const DecoratedVertex& target);

	private:
		Path(const DecoratedVertex& source, const DecoratedVertex& target);

		void collectIdentifiersOnThePath(const DecoratedVertex& target);
		void printIdentifiers(std::ostream& out) const;
		void printLength(std::ostream& out) const;
		void swapContentsWith(Path p);

	private:
		std::forward_list<String> identifiers;
		Distance length;
	};

public:
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = default;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = default;
	virtual ~ShortestPathAlgorithm() = default;
	
	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& target) = 0;
	const String& getID() const;
	void setID(const char* id);

protected:
	ShortestPathAlgorithm(const char* id);

	void initialiseVerticesOf(Graph& graph) const;
	virtual void initialiseVertex(Vertex& vertex) const = 0;
	virtual void initialiseSource(Vertex& source) const;

private:
	String id;
};

#endif //__SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__