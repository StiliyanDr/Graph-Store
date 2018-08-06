
template <class ConcreteGraph>
inline ConcreteGraphCreator<ConcreteGraph>::ConcreteGraphCreator(const char* criterion) :
	GraphCreator(criterion)
{
}

template <class ConcreteGraph>
inline std::unique_ptr<Graph> ConcreteGraphCreator<ConcreteGraph>::createGraph(const String& identifier) const
{
	return std::unique_ptr<Graph>(new ConcreteGraph(identifier));
}
