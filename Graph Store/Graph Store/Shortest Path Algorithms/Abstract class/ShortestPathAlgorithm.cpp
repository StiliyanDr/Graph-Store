#include "ShortestPathAlgorithm.h"
#include "Invocer/Invocer.h"
#include <algorithm>
#include <stdexcept>

ShortestPathAlgorithm::Path::Path(const DecoratedVertex& source,
	                              const DecoratedVertex& target) :
	length(target.distanceToSource)
{
	if (existsPathBetween(source, target))
	{
		collectIDsOnThePath(target);
	}
}

bool ShortestPathAlgorithm::Path::existsPathBetween(const DecoratedVertex& source,
													const DecoratedVertex& target)
{
	return *(source.originalVertex) == *(target.originalVertex)
		   || target.parent != nullptr;
}

void ShortestPathAlgorithm::Path::collectIDsOnThePath(const DecoratedVertex& target)
{
	auto current = &target;

	while (current != nullptr)
	{
		ids.push_front(current->originalVertex->getID());

		current = current->parent;
	}
}

ShortestPathAlgorithm::Path::Path(Path&& source) :
	ids(std::move(source.ids)),
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
	std::swap(ids, p.ids);
	std::swap(length, p.length);
}

std::ostream& operator<<(std::ostream& out,
	                     const ShortestPathAlgorithm::Path& p)
{
	p.print(out);

	return out;
}

void ShortestPathAlgorithm::Path::print(std::ostream& out) const
{
	printIDs(out);
	printLength(out);
}

void ShortestPathAlgorithm::Path::printIDs(std::ostream& out) const
{
	if (!ids.empty())
	{
		out << "Path: ";

		std::for_each(ids.cbegin(),
			          ids.cend(),
			          [&out](const auto& id)
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
	out << "Path length: " << length;
}

ShortestPathAlgorithm::ShortestPathAlgorithm(String id)
{
	setID(std::move(id));
}

void ShortestPathAlgorithm::setID(String id)
{
	if (id != ""_s)
	{
		this->id = std::move(id);
	}
	else
	{
		throw std::invalid_argument("Invalid algorithm id!");
	}
}

ShortestPathAlgorithm::Path
ShortestPathAlgorithm::findShortestPath(const Graph& graph,
										const Graph::Vertex& source,
										const Graph::Vertex& target)
{
	auto resourceReleaser =
		Invocer{[this]() noexcept { cleanUp(); }};
	initialise(graph, source, target);
	execute(graph, source, target);
	auto path = createPathBetween(source, target);

	return path;
}

ShortestPathAlgorithm::Path
ShortestPathAlgorithm::createPathBetween(const Graph::Vertex& source,
										 const Graph::Vertex& target)
{
	return Path(getDecoratedVersionOf(source),
		        getDecoratedVersionOf(target));
}

void ShortestPathAlgorithm::decorateVerticesOf(const Graph& g)
{
	auto iterator = g.getConstIteratorOfVertices();
	
	forEach(*iterator, [this](const auto& vertex)
	{
		addDecoratedVersionOf(vertex);
	});
}

void ShortestPathAlgorithm::initialiseSource(DecoratedVertex& source)
{
	source.parent = nullptr;
	source.distanceToSource = 0;
}

const String& ShortestPathAlgorithm::getID() const noexcept
{
	return id;
}
