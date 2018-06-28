#include "stdafx.h"
#include "GraphCreator.h"

GraphCreator::GraphCreator(const char* criterion)
{
	setCriterion(criterion);
}

void GraphCreator::setCriterion(String criterion)
{
	assert(criterion != String(""));

	this->criterion = std::move(criterion);
}

const String& GraphCreator::getCriterion() const
{
	return criterion;
}
