#include "GraphCreator.h"

GraphCreator::GraphCreator(const char* criterion)
{
	setCriterion(criterion);
}

void GraphCreator::setCriterion(String criterion)
{
	assert(criterion != ""_s);

	this->criterion = std::move(criterion);
}

const String& GraphCreator::getCriterion() const
{
	return criterion;
}
