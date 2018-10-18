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

std::unique_ptr<Graph> GraphFactory::createGraph(const String& criterion, const String& id)
{
	const GraphCreator& creator = getCreatorByCriterion(criterion);

	return creator.createGraph(id);
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
	ConstIterator iterator = creators.getConstIterator();
	const GraphCreator* creator;

	while (iterator)
	{
		creator = *iterator;

		if (creator->getCriterion() == criterion)
		{
			return creator;
		}

		++iterator;
	}

	return nullptr;
}

void GraphFactory::addCreator(const GraphCreator& creator)
{
	assert(searchForCreator(creator.getCriterion()) == nullptr);

	creators.add(&creator);
}
