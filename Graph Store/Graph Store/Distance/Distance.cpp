#include "Distance.h"

const Distance Distance::infinity;

const Distance& Distance::getInfinity() noexcept
{
	return infinity;
}

Distance::Distance() noexcept :
	distance(0),
	isInfinity(true)
{
}

Distance::Distance(unsigned distance) noexcept :
	distance(distance),
	isInfinity(false)
{
}

const Distance operator+(const Distance& lhs,
	                     const Distance& rhs) noexcept
{
	auto result = lhs;
	result += rhs;

	return result;
}

Distance&
Distance::operator+=(const Distance& rhs) noexcept
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

bool operator!=(const Distance& lhs,
	            const Distance& rhs) noexcept
{
	return !(lhs == rhs);
}

bool operator==(const Distance& lhs,
	            const Distance& rhs) noexcept
{
	return !(lhs.isInfinity || rhs.isInfinity)
		   && (lhs.distance == rhs.distance);
}

bool operator>(const Distance& lhs,
	           const Distance& rhs) noexcept
{
	return rhs < lhs;
}

bool operator<=(const Distance& lhs,
	            const Distance& rhs) noexcept
{
	return !(rhs < lhs);
}

bool operator>=(const Distance& lhs,
	            const Distance& rhs) noexcept
{
	return !(lhs < rhs);
}

bool operator<(const Distance& lhs,
	           const Distance& rhs) noexcept
{
	return (!(lhs.isInfinity || rhs.isInfinity)
		    && (lhs.distance < rhs.distance))
		   ||
		   (!lhs.isInfinity && rhs.isInfinity);
}

std::ostream& operator<<(std::ostream& output,
	                     const Distance& d)
{
	if (!d.isInfinity)
	{
		output << d.distance;
	}
	else
	{
		output << "infinity";
	}
	
	return output;
}
