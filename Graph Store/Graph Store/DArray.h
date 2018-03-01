#ifndef __DARRAY_HEADER_INCLUDED__
#define __DARRAY_HEADER_INCLUDED__

template <class T>
class DArray
{
public:
	explicit DArray(int size = 16, int count = 0);
	DArray(const T* data, int size);
	DArray(DArray<T>&& source);
	DArray(const DArray<T>& source);
	DArray<T>& operator=(DArray<T>&& rhs);
	DArray<T>& operator=(const DArray<T>& rhs);
	virtual ~DArray();

	virtual void add(const T& item);
	virtual void add(T&& item);
	virtual void remove(int index);
	virtual int find(const T& what) const;

	void addAt(int index, const T& item);
	void addAt(int index, T&& item);
	void empty();
	void ensureSize(int size);
	bool isEmpty() const;

	int getSize() const;
	int getCount() const;

public:
	T& operator[](int index);
	const T& operator[](int index) const;
	DArray<T>& operator+=(const T& item);
	DArray<T>& operator+=(T&& item);
	DArray<T>& operator+=(const DArray<T>& rhs);
	DArray<T>& operator+=(DArray<T>&& rhs);

protected:
	void shiftLeft(int index);
	void shiftRight(int index);
	void resize(int newSize);
	void checkSize();
	T* getData();
	void setCount(int count);
	void setSize(int size);

private:
	int count, size;
	T* data;

private:
	void nullify();
	void clean();
	void copyFrom(const DArray<T>& source);
};

template <class T>
DArray<T> operator+(const DArray<T>& lhs, const DArray<T>& rhs);

template <class T>
DArray<T> operator+(const T& lhs, const DArray<T>& rhs);

template <class T>
DArray<T> operator+(const DArray<T>& lhs, const T& rhs);

#include "DArray.hpp"

#endif //__DARRAY_HEADER_INCLUDED__