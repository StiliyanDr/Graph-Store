
template <class Item, class Key, class HandleUpdator>
inline PriorityQueue<Item, Key, HandleUpdator>::Handle::Handle() :
	Handle(-1)
{
}

template <class Item, class Key, class HandleUpdator>
inline PriorityQueue<Item, Key, HandleUpdator>::Handle::Handle(long index) :
	index(index)
{
}

template <class Item, class Key, class HandleUpdator>
inline bool PriorityQueue<Item, Key, HandleUpdator>::Handle::isValid() const
{
	return index != -1;
}
