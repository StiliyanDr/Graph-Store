#include <stdexcept>
#include <cassert>
#include <utility>


///
///Returns an array containing both lhs' and rhs' elements.
///
template <class T>
DArray<T> operator+(const DArray<T>& lhs, const DArray<T>& rhs)
{
	DArray<T> tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Returns a copy of rhs that now contains lhs.
///
template <class T>
inline DArray<T> operator+(const T& lhs, const DArray<T>& rhs)
{
	return rhs + lhs;
}


///
///Returns a copy of lhs that now contains rhs.
///
template <class T>
DArray<T> operator+(const DArray<T>& lhs, const T& rhs)
{
	DArray<T> tmp(lhs);
	tmp += rhs;

	return tmp;
}


///
///Creates an object with size elements,
///count of which are considered as used.
///
template <class T>
DArray<T>::DArray(int size, int count) :
	data(nullptr)
{
	setSize(size);
	setCount(count);

	if(size > 0)
		data = new T[size];
}


///
///Creates a dynamic array with size elements
///that are copies of the passed array's first
///size elements. If <data> is nullptr or it is
///not but <size> is less than one, then the 
///resulting object is empty.
///
template <class T>
DArray<T>::DArray(const T* data, int size)
{
	nullify();

	if (data && size > 0)
	{
		this->data = new T[size];

		this->size = count = size;

		for (int i = 0; i < size; ++i)
			this->data[i] = data[i];
	}
}


///
///Moves source's array into the current object
///and leaves source empty.
///
template <class T>
DArray<T>::DArray(DArray<T>&& source) :
	count(source.count), size(source.size), data(source.data)
{
	source.nullify();
}


template <class T>
inline DArray<T>::DArray(const DArray<T>& source) :
	data(nullptr)
{
	copyFrom(source);
}


///
///Moves rhs' array into the current object
///and leaves rhs empty.
///
template <class T>
DArray<T>& DArray<T>::operator=(DArray<T>&& rhs)
{
	if (this != &rhs)
	{
		//Release old memory.
		clean();					
		
		//Move rhs' internals into the object.
		data = rhs.data;			
		count = rhs.count;
		size = rhs.size;

		rhs.nullify();
	}
	return *this;
}


template <class T>
DArray<T>& DArray<T>::operator=(const DArray<T>& rhs)
{
	if (this != &rhs)
	{
		copyFrom(rhs);
	}
	return *this;
}


template <class T>
inline DArray<T>::~DArray()
{
	clean();
}


///
///Adds elem at the end of the array.
///
template <class T>
void DArray<T>::add(const T& elem)
{
	//First ensure there is room for elem.
	checkSize();

	data[count++] = elem;
}


///
///Adds elem at the end of the array.
///
///elem is moved, provided that T
///can be move-assigned.
///
template <class T>
void DArray<T>::add(T&& elem)
{
	//First ensure there is room for elem.
	checkSize();

	data[count++] = std::move(elem);
}


///
///Removes the element at position index in
///the array by shifting all the elements to
///its right with one position to the left.
///
template <class T>
void DArray<T>::remove(int index)
{
	if (index >= 0 && index < count)
	{
		--count;
		shiftLeft(index);
	}
	else
		throw std::out_of_range("Index out of range!");
}


///
///Uses linear search to find an element in
///the array with the same value as what and
///returns its index or -1 if there is no such
///element in the array.
///
template <class T>
int DArray<T>::find(const T& what) const
{
	for (int i = 0; i < count; ++i)
	{
		if (data[i] == what)
			return i;
	}

	return -1;
}


///
///Shifts the elements to the right of index 
///with one position to the right and adds 
///elem at position index in the array.
///
template <class T>
void DArray<T>::addAt(int index, const T& elem)
{
	if (index >= 0 && index <= count)
	{
		//Make room for the new element.
		shiftRight(index);
		data[index] = elem;
		++count;
	}
	else
		throw std::out_of_range("Index out of range!");
}


///
///Shifts the elements to the right of index with
///one position to the right and adds elem at position
///index in the array. elem is moved into the element 
///at that position, provided that T can be move-assigned.
///
template <class T>
void DArray<T>::addAt(int index, T&& elem)
{
	if (index >= 0 && index <= count)
	{
		//Make room for the new element.
		shiftRight(index);
		data[index] = std::move(elem);
		++count;
	}
	else
		throw std::out_of_range("Index out of range!");
}


///
///Empties the array.
///
template <class T>
void DArray<T>::empty()
{
	clean();
	nullify();
}


///
///If the number of allocated elements is less
///than the desired number, resize the array.
///
template <class T>
inline void DArray<T>::ensureSize(int size)
{
	if (size > this->size)
		resize(size);
}


///
///Checks whether no element in the
///array is considered as used.
///
template <class T>
inline bool DArray<T>::isEmpty() const
{
	return count == 0;
}


///
///Returns the size of the array
///(the number of allocated elements).
///
template <class T>
inline int DArray<T>::getSize() const
{
	return size;
}


///
///Returns the number of elements in
///the array that are considered as used.
///
template <class T>
inline int DArray<T>::getCount() const
{
	return count;
}


///
///Returns a reference to the element
///at position index in the array.
///
template <class T>
T& DArray<T>::operator[](int index)
{
	if (index >= 0 && index < count)
		return data[index];
	else
		throw std::out_of_range("Index out of range!");
}


///
///Returns a const reference to the element
///at position index in the array.
///
template <class T>
const T& DArray<T>::operator[](int index) const
{
	if (index >= 0 && index < count)
		return data[index];
	else
		throw std::out_of_range("Index out of range!");
}


///
///See add.
///
template <class T>
DArray<T>& DArray<T>::operator+=(const T& elem)
{
	add(elem);

	return *this;
}


///
///See add.
///
template <class T>
DArray<T>& DArray<T>::operator+=(T&& elem)
{
	add(std::move(elem));

	return *this;
}


///
///Adds copies of rhs' elements to the array.
///
template <class T>
DArray<T>& DArray<T>::operator+=(const DArray<T>& rhs)
{
	int used = rhs.getCount();

	for (int i = 0; i < used; ++i)
		add(rhs[i]);

	return *this;
}


///
///Adds rhs' elements to the array.
///If T has move-assignment operator, each of
///the elements is moved into the current array,
///otherwise copies of the elements are added 
///to the array.
///
template <class T>
DArray<T>& DArray<T>::operator+=(DArray<T>&& rhs)
{
	int used = rhs.getCount();

	for (int i = 0; i < used; ++i)
		add(std::move(rhs[i]));

	return *this;
}


///
///Shifts the elements to the right of index with
///one position to the left, until count is reached.
///
///count must be correct when this function is
///called from remove.
///
///Pass the index to be "filled".
///
template <class T>
void DArray<T>::shiftLeft(int index)
{
	assert(index >= 0);

	for (int i = index; i < count; ++i)
		data[i] = data[i + 1];
}


///
///Starting from the last element considered as used,
///shifts the elements in the array with one position
///to the right until index is reached.
///
///Pass the index to be "emptied" as argument.
///
template <class T>
void DArray<T>::shiftRight(int index)
{
	assert(index >= 0);

	//Extend the array if necessary.
	checkSize();

	for (int i = count; i > index; --i)
		data[i] = data[i - 1];
}


///
///Resizes the array so that it has newSize elements.
///If newSize is less than count, only the first
///newSize of the elements remain in the array.
///
template <class T>
void DArray<T>::resize(int newSize)
{
	assert(newSize > 0);

	T* newData = new T[newSize];

	int used = (newSize < count) ? newSize : count;

	for (int i = 0; i < used; ++i)
		newData[i] = data[i];

	clean();
	data = newData;
	size = newSize;
	count = used;
}


///
///If all of the allocated elements
///are considered as used, double the
///array's size.
///
template <class T>
void DArray<T>::checkSize()
{
	assert(size >= 0);
	assert(count <= size);

	if (count >= size)
		resize(size ? 2 * size : 2);
}


template <class T>
inline T* DArray<T>::getData()
{
	return data;
}


///
///Sets the number of elements in the array
///that are considered as used.
///
template <class T>
void DArray<T>::setCount(int count)
{
	if (count >= 0 && count <= size)
		this->count = count;
	else
		throw std::invalid_argument("Count must be >= 0 and <= size!");
}


///
///Sets the number of allocated elements.
///
template <class T>
void DArray<T>::setSize(int size)
{
	if (size >= 0)
		this->size = size;
	else
		throw std::invalid_argument("Size can not be negative!");
}


///
///Sets the members with null values.
///
template <class T>
void DArray<T>::nullify()
{
	data = nullptr;
	count = size = 0;
}


///
///Releases memory.
///
template <class T>
inline void DArray<T>::clean()
{
	delete[] data;
}


///
///Copies source's array, releases old memory
///and stores the copy. size and count are
///assigned their corresponding values from source.
///
template <class T>
void DArray<T>::copyFrom(const DArray<T>& source)
{
	T* newData = nullptr;

	if (source.data)
	{
		newData = new T[source.size];

		for (int i = 0; i < source.count; ++i)
			newData[i] = source.data[i];
	}
		
	clean();
	data = newData;
	size = source.size;
	count = source.count;
}
