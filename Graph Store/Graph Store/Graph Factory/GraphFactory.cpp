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

std::unique_ptr<Graph> GraphFactory::createGraph(const String& criterion, const String& identifier)
{
	const GraphCreator& creator = getCreatorByCriterion(criterion);

	return creator.createGraph(identifier);
}

const GraphCreator& GraphFactory::getCreatorByCriterion(const String& criterion)
{
	const GraphCreator* creator = searchForCreator(criterion);

	if (creator != nullptr)
	{
		return *creator;
	}
	else
	{
		throw InvalidGraphTypeException("No type of graph corresponds to the criterion: " + criterion);
	}
}

const GraphCreator* GraphFactory::searchForCreator(const String& criterion)
{
	Iterator iterator = creators.getIteratorToFirst();
	const GraphCreator* creator;

	while (iterator.isValid())
	{
		creator = *iterator;

		if (creator->getCriterion() == criterion)
		{
			return creator;
		}

		iterator.advance();
	}

	return nullptr;
}

void GraphFactory::addCreator(const GraphCreator& creator)
{
	assert(searchForCreator(creator.getCriterion()) == nullptr);

	creators.add(&creator);
}
