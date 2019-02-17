#include "GraphCreator.h"

GraphCreator::GraphCreator(const char* criterion)
{
	setCriterion(criterion);
}

void GraphCreator::setCriterion(String criterion)
{
	if (criterion != ""_s)
	{
		this->criterion = std::move(criterion);
	}
	else
	{
		throw std::invalid_argument("Invalid criterion!");
	}
}

const String& GraphCreator::getCriterion() const
{
	return criterion;
}
