#include "PathPrinting.h"
#include "../../Graph/Vertex/Vertex.h"
#include <iostream>

void printPathAndItsLength(const Vertex& start, const Vertex& end)
{
	printPath(start, end);
	printLength(end);
}

void printPath(const Vertex& start, const Vertex& end)
{
	if (start == end)
	{
		std::cout << start.getIdentifier();
	}
	else if (end.getParent() != nullptr)
	{
		printPath(start, *(end.getParent()));
		std::cout << ", " << end.getIdentifier();
	}
	else
	{
		std::cout << "No such path exists!";
	}
}

void printLength(const Vertex& end)
{
	std::cout << "\nPath length: " << end.getDistance() << '\n';
}
