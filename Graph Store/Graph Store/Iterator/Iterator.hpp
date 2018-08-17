
template <class Iterator, class Function>
void forEach(Iterator& iterator, const Function& function)
{
	while (iterator.isValid())
	{
		function(iterator.getCurrentItem());

		iterator.advance();
	}
}
