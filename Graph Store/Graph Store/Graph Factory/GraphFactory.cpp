#include "GraphFactory.h"
#include "Invalid Graph Type Exception/InvalidGraphTypeException.h"

std::unique_ptr<Graph> GraphFactory::createGraph(const char* criterion, const char* identifier)
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
	Collection& creators = getCreators();
	Iterator creatorsIterator = creators.getIteratorToFirst();
	const GraphCreator* creator;

	while (creatorsIterator.isValid())
	{
		creator = *creatorsIterator;

		if (creator->getCriterion() == criterion)
		{
			return creator;
		}

		creatorsIterator.advance();
	}

	return nullptr;
}

void GraphFactory::addCreator(const GraphCreator& creator)
{
	assert(!searchForCreator(creator.getCriterion()));

	getCreators().add(&creator);
}

GraphFactory::Collection& GraphFactory::getCreators()
{
	static Collection creators(INITIAL_COLLECTION_SIZE);
	
	return creators;
}
