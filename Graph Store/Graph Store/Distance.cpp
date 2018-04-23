#include "stdafx.h"
#include "Distance.h"


Distance::Distance(unsigned distance)
{
	setDistance(distance);
}


Distance& Distance::operator=(unsigned distance)
{
	setDistance(distance);
}


Distance operator+(const Distance& lhs, const Distance& rhs)
{
	Distance result = Distance::infinity;

	if (!(lhs.isInfinity || rhs.isInfinity))
	{
		result.setDistance(lhs.distance + rhs.distance);
	}

	return result;
}


bool operator!=(const Distance& lhs, const Distance& rhs)
{
	return !(lhs == rhs);
}


bool operator==(const Distance& lhs, const Distance& rhs)
{
	return !(lhs.isInfinity || rhs.isInfinity) && (lhs.distance == rhs.distance);
}


bool operator>(const Distance& lhs, const Distance& rhs)
{
	return rhs < lhs;
}


bool operator<=(const Distance& lhs, const Distance& rhs)
{
	return !(rhs < lhs);
}


bool operator>=(const Distance& lhs, const Distance& rhs)
{
	return !(lhs < rhs);
}


bool operator<(const Distance& lhs, const Distance& rhs)
{
	return (!(lhs.isInfinity || rhs.isInfinity) && (lhs.distance < rhs.distance))
		|| (!lhs.isInfinity && rhs.isInfinity);
}


std::ostream& operator<<(std::ostream& output, const Distance& distance)
{
	if (!distance.isInfinity)
	{
		output << distance.distance;
	}
	else
	{
		output << "infinity";
	}
	
	return output;
}


void Distance::setDistance(unsigned distance)
{
	this->distance = distance;
	isInfinity = false;
}
