#ifndef __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__

#include "String/String.h"
#include "Distance/Distance.h"
#include "Graph/Abstract class/Graph.h"
#include <forward_list>

class ShortestPathAlgorithm
{
protected:
	struct DecoratedVertex
	{
		DecoratedVertex() = default;
		explicit DecoratedVertex(const Graph::Vertex& originalVertex) noexcept :
			originalVertex(&originalVertex),
			parent(nullptr),
			distanceToSource(Distance::getInfinity())
		{
		}

		const Graph::Vertex* originalVertex;
		const DecoratedVertex* parent;
		Distance distanceToSource;
	};

public:
	class Path
	{
		friend class ShortestPathAlgorithm;

	public:
		Path(const Path&) = default;
		Path& operator=(const Path&) = default;
		Path(Path&& source);
		Path& operator=(Path&& rhs);

		void print(std::ostream& out) const;

	private:
		static bool existsPathBetween(const DecoratedVertex& source,
			                          const DecoratedVertex& target);

	private:
		Path(const DecoratedVertex& source,
			 const DecoratedVertex& target);

		void collectIDsOnThePath(const DecoratedVertex& target);
		void printIDs(std::ostream& out) const;
		void printLength(std::ostream& out) const;
		void swapContentsWith(Path p);

	private:
		std::forward_list<String> ids;
		Distance length;
	};

public:
	virtual ~ShortestPathAlgorithm() = default;

	Path findShortestPath(const Graph& g,
		                  const Graph::Vertex& source,
		                  const Graph::Vertex& target);
	const String& getID() const noexcept;

protected:
	static void initialiseSource(DecoratedVertex& source);

protected:
	explicit ShortestPathAlgorithm(String id);
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = default;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = default;

	virtual void addDecoratedVersionOf(const Graph::Vertex& v) = 0;
	virtual DecoratedVertex& getDecoratedVersionOf(const Graph::Vertex& v) = 0;
	virtual void decorateVerticesOf(const Graph& g);

private:
	virtual void initialise(const Graph& g,
							const Graph::Vertex& source,
							const Graph::Vertex& target) = 0;
	virtual void execute(const Graph& g,
						 const Graph::Vertex& source,
						 const Graph::Vertex& target) = 0;
	virtual void cleanUp() = 0;
	Path createPathBetween(const Graph::Vertex& source,
		                   const Graph::Vertex& target);
	void setID(String id);

private:
	String id;
};

std::ostream& operator<<(std::ostream& out,
	                     const ShortestPathAlgorithm::Path& p);

#endif //__SHORTEST_PATH_ALGORITHM_HEADER_INCLUDED__