#include "ShortestPathAlgorithm.h"
#include "../../Graph/Abstract class/Graph.h"
#include "../../Graph/Vertex/Vertex.h"
#include <algorithm>
#include <stdexcept>

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

ShortestPathAlgorithm::ShortestPathAlgorithm(const String& id)
{
	setID(id);
}

void ShortestPathAlgorithm::setID(const String& id)
{
	if (id != ""_s)
	{
		this->id = id;
	}
	else
	{
		throw std::invalid_argument("Invalid algorithm id!");
	}
}

ShortestPathAlgorithm::Path
ShortestPathAlgorithm::findShortestPath(const Graph& graph,
										const Vertex& source,
										const Vertex& target)
{
	try
	{
		initialise(graph, source, target);
		execute(graph, source, target);
		Path result = createPathBetween(source, target);
		cleanUp();

		return result;
	}
	catch (std::bad_alloc&)
	{
		cleanUp();
		throw;
	}
}

ShortestPathAlgorithm::Path
ShortestPathAlgorithm::createPathBetween(const Vertex& source, const Vertex& target)
{
	DecoratedVertex& decoratedSource = getDecoratedVersionOf(source);
	DecoratedVertex& decoratedTarget = getDecoratedVersionOf(target);

	return Path(decoratedSource, decoratedTarget);
}

void ShortestPathAlgorithm::decorateVerticesOf(const Graph& graph)
{
	forEach(*(graph.getConstIteratorOfVertices()), [&](const Vertex* vertex)
	{
		addDecoratedVersionOf(*vertex);
	});
}

void ShortestPathAlgorithm::initialiseSource(DecoratedVertex& source)
{
	source.parent = nullptr;
	source.distanceToSource = 0;
}

const String& ShortestPathAlgorithm::getID() const
{
	return id;
}
