#include "ShortestPathAlgorithm.h"
#include <assert.h>
#include "../../Graph/Abstract class/Graph.h"
#include "../../Graph/Vertex/Vertex.h"
#include <algorithm>

ShortestPathAlgorithm::Path::Path(const DecoratedVertex& source, const DecoratedVertex& target) :
	length(target.distanceToSource)
{
	if (existsPathBetween(source, target))
	{
		collectIdentifiersOnThePath(target);
	}
}

bool ShortestPathAlgorithm::Path::existsPathBetween(const DecoratedVertex& source,
													const DecoratedVertex& target)
{
	return source.originalVertex == target.originalVertex
		   || target.parent != nullptr;
}

void ShortestPathAlgorithm::Path::collectIdentifiersOnThePath(const DecoratedVertex& target)
{
	const DecoratedVertex* current = &target;

	while (current != nullptr)
	{
		identifiers.push_front(current->originalVertex.getID());

		current = current->parent;
	}
}

ShortestPathAlgorithm::Path::Path(Path&& source) :
	identifiers(std::move(source.identifiers)),
	length(std::move(source.length))
{
	source.length = Distance::getInfinity();
}

ShortestPathAlgorithm::Path&
ShortestPathAlgorithm::Path::operator=(Path&& rhs)
{
	if (this != &rhs)
	{
		swapContentsWith(std::move(rhs));
	}

	return *this;
}

void ShortestPathAlgorithm::Path::swapContentsWith(Path p)
{
	std::swap(identifiers, p.identifiers);
	std::swap(length, p.length);
}

void ShortestPathAlgorithm::Path::print(std::ostream& out) const
{
	printIdentifiers(out);
	printLength(out);
}

void ShortestPathAlgorithm::Path::printIdentifiers(std::ostream& out) const
{
	if (!identifiers.empty())
	{
		out << "Path: ";

		std::for_each(identifiers.cbegin(), identifiers.cend(), [&](const String& id)
		{
			out << id << ", ";
		});

		out << '\n';
	}
	else
	{
		out << "No such path exists!\n";
	}
}

void ShortestPathAlgorithm::Path::printLength(std::ostream& out) const
{
	out << "Path length: " << length << ".\n";
}

ShortestPathAlgorithm::ShortestPathAlgorithm(const char* id)
{
	setID(id);
}

void ShortestPathAlgorithm::setID(const char* id)
{
	assert(id != nullptr && *id);

	this->id = id;
}

const String& ShortestPathAlgorithm::getID() const
{
	return id;
}

void ShortestPathAlgorithm::initialiseVerticesOf(Graph& graph) const
{
	std::unique_ptr<Iterator<Vertex*>> iterator = graph.getIteratorOfVertices();

	forEach(*iterator, [&](Vertex* v)
	{
		assert(v != nullptr);
		initialiseVertex(*v);
	});
}

void ShortestPathAlgorithm::initialiseVertex(Vertex& vertex) const
{
	vertex.markAsNotVisited();
	vertex.setParent(nullptr);
	vertex.setDistance(Distance::getInfinity());
}

void ShortestPathAlgorithm::initialiseSource(Vertex& source) const
{
	source.setDistance(0);
}
