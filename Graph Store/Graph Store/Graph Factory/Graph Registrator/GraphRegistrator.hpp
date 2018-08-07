#include "../GraphFactory.h"

template <class ConcreteGraph>
GraphRegistrator<ConcreteGraph>::GraphRegistrator(const char* graphType) :
	graphCreator(graphType)
{
	GraphFactory::instance().addCreator(graphCreator);
}
