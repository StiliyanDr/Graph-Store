#include "stdafx.h"
#include "Distance.h"

const Distance Distance::infinity;

const Distance& Distance::getInfinity()
{
	return infinity;
}

Distance::Distance() :
	isInfinity(true)
{
}

Distance::Distance(unsigned distance)
{
	setDistance(distance);
}

Distance& Distance::operator=(unsigned distance)
{
	setDistance(distance);

	return *this;
}

void Distance::setDistance(unsigned distance)
{
	this->distance = distance;
	isInfinity = false;
}

Distance operator+(const Distance& lhs, const Distance& rhs)
{
	Distance result = lhs;
	result += rhs;

	return result;
}

Distance& Distance::operator+=(const Distance& rhs)
{
	if (!(this->isInfinity || rhs.isInfinity))
	{
		this->distance += rhs.distance;
	}
	else
	{
		this->isInfinity = true;
	}

	return *this;
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
