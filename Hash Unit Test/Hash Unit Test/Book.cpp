#include "stdafx.h"
#include "Book.h"

Book::Book(const char* title)
{
	setTitle(title);
}

void Book::setTitle(const char* title)
{
	this->title = title;
}

const String& Book::getTitle() const
{
	return title;
}
