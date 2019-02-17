#include "GraphFactory.h"
#include "Invalid Graph Type Exception/InvalidGraphTypeException.h"

GraphFactory::GraphFactory() :
	creators(INITIAL_COLLECTION_SIZE)
{
}

GraphFactory& GraphFactory::instance()
{
	static GraphFactory theInstance;
	
	return theInstance;
}

std::unique_ptr<Graph>
GraphFactory::createGraph(const String& criterion, const String& id)
{
	const GraphCreator& creator =
		getCreatorByCriterion(criterion);

	return creator.createGraph(id);
}

const GraphCreator&
GraphFactory::getCreatorByCriterion(const String& criterion)
{
	Collection::ConstIterator iterator =
		searchForCreator(criterion);

	if (iterator)
	{
		return *(*iterator);
	}
	else
	{
		throw InvalidGraphTypeException("No type of graph corresponds to the criterion: " + criterion);
	}
}

GraphFactory::Collection::ConstIterator
GraphFactory::searchForCreator(const String& criterion) const
{
	return forEachUntil(creators.getConstIterator(),
		                [&](const GraphCreator* c)
	{
		return c->getCriterion() == criterion;
	});
}

void GraphFactory::addCreator(const GraphCreator& c)
{
	if (!searchForCreator(c.getCriterion()))
	{
		creators.add(&c);
	}
	else
	{
		throw std::logic_error("Duplicate creator!");
	}
}
